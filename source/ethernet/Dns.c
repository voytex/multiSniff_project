// Arduino DNS client for WizNet5100-based Ethernet shield
// (c) Copyright 2009-2010 MCQN Ltd.
// Released under Apache License, version 2.0

#include <source/driverlib/w5500/w5500.h>
#include "EthernetUdp.h"
#include "util.h"

#include "Dns.h"
#include <string.h>
//#include <stdlib.h>
#ifdef ARDUINO
#include "Arduino.h"
#else
#include "ArduinoCompatibility.h"
#endif


#define SOCKET_NONE	255
// Various flags and header field values for a DNS message
#define UDP_HEADER_SIZE	8
#define DNS_HEADER_SIZE	12
#define TTL_SIZE        4
#define QUERY_FLAG               (0)
#define RESPONSE_FLAG            (1<<15)
#define QUERY_RESPONSE_MASK      (1<<15)
#define OPCODE_STANDARD_QUERY    (0)
#define OPCODE_INVERSE_QUERY     (1<<11)
#define OPCODE_STATUS_REQUEST    (2<<11)
#define OPCODE_MASK              (15<<11)
#define AUTHORITATIVE_FLAG       (1<<10)
#define TRUNCATION_FLAG          (1<<9)
#define RECURSION_DESIRED_FLAG   (1<<8)
#define RECURSION_AVAILABLE_FLAG (1<<7)
#define RESP_NO_ERROR            (0)
#define RESP_FORMAT_ERROR        (1)
#define RESP_SERVER_FAILURE      (2)
#define RESP_NAME_ERROR          (3)
#define RESP_NOT_IMPLEMENTED     (4)
#define RESP_REFUSED             (5)
#define RESP_MASK                (15)
#define TYPE_A                   (0x0001)
#define CLASS_IN                 (0x0001)
#define LABEL_COMPRESSION_MASK   (0xC0)
// Port number that DNS servers listen on
#define DNS_PORT        53

// Possible return codes from ProcessResponse
#define SUCCESS          1
#define TIMED_OUT        -1
#define INVALID_SERVER   -2
#define TRUNCATED        -3
#define INVALID_RESPONSE -4

void DNSClient_begin(DNSClient* dns, IPAddress aDNSServer)
{
    dns->iDNSServer.dword = aDNSServer.dword;
	dns->iRequestId = 0;
	EthernetUDP_begin_init(&dns->iUdp);
}


int DNSClient_inet_aton(const char* aIPAddrString, IPAddress* aResult)
{
    // See if we've been given a valid IP address
    const char* p =aIPAddrString;
    while (*p &&
           ( (*p == '.') || (*p >= '0') || (*p <= '9') ))
    {
        p++;
    }

    if (*p == '\0')
    {
        // It's looking promising, we haven't found any invalid characters
        p = aIPAddrString;
        int segment =0;
        int segmentValue =0;
        while (*p && (segment < 4))
        {
            if (*p == '.')
            {
                // We've reached the end of a segment
                if (segmentValue > 255)
                {
                    // You can't have IP address segments that don't fit in a byte
                    return 0;
                }
                else
                {
                    aResult->bytes[segment] = (uint8_t)segmentValue;
                    segment++;
                    segmentValue = 0;
                }
            }
            else
            {
                // Next digit
                segmentValue = (segmentValue*10)+(*p - '0');
            }
            p++;
        }
        // We've reached the end of address, but there'll still be the last
        // segment to deal with
        if ((segmentValue > 255) || (segment > 3))
        {
            // You can't have IP address segments that don't fit in a byte,
            // or more than four segments
            return 0;
        }
        else
        {
            aResult->bytes[segment] = (uint8_t)segmentValue;
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

int DNSClient_getHostByName(DNSClient* dns, const char* aHostname, IPAddress* aResult)
{
    int ret =0;

    // See if it's a numeric IP address
    if (DNSClient_inet_aton(aHostname, aResult))
    {
        // It is, our work here is done
        return 1;
    }

	// Check we've got a valid DNS server to use
    if (dns->iDNSServer.dword == 0)
    {
        return INVALID_SERVER;
	}

	// Find a socket to use
    if (EthernetUDP_begin(&dns->iUdp,1024+(millis() & 0xF)) == 1)
    {

		// Try up to three times
        int retries = 0;
//        while ((retries < 3) && (ret <= 0))
        {

			// Send DNS request
            ret = EthernetUDP_beginPacket_ip(&dns->iUdp,dns->iDNSServer, DNS_PORT);
            if (ret != 0)
            {

				// Now output the request data
                ret = DNSClient_BuildRequest(dns, aHostname);
                if (ret != 0)
                {

					// And finally send the request
                    ret = EthernetUDP_endPacket(&dns->iUdp);
                    if (ret != 0)
                    {

						// Now wait for a response
                        int wait_retries = 0;
                        ret = TIMED_OUT;
                        while ((wait_retries < 3) && (ret == TIMED_OUT))
                        {

							ret = DNSClient_ProcessResponse(dns, 5000, aResult);
                            wait_retries++;
                        }
                    }
                }
            }
            retries++;
        }

        // We're done with the socket now
		EthernetUDP_stop(&dns->iUdp);
    }


    return ret;
}

uint16_t DNSClient_BuildRequest(DNSClient* dns, const char* aName)
{
    // Build header
    //                                    1  1  1  1  1  1
    //      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                      ID                       |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    QDCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    ANCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    NSCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    ARCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // As we only support one request at a time at present, we can simplify
    // some of this header
	dns->iRequestId = millis(); // generate a random ID
    uint16_t twoByteBuffer;

    // FIXME We should also check that there's enough space available to write_byte to, rather
    // FIXME than assume there's enough space (as the code does at present)
	EthernetUDP_write(&dns->iUdp,(uint8_t*)&dns->iRequestId, sizeof(dns->iRequestId));

    twoByteBuffer = htons(QUERY_FLAG | OPCODE_STANDARD_QUERY | RECURSION_DESIRED_FLAG);
	EthernetUDP_write(&dns->iUdp,(uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    twoByteBuffer = htons(1);  // One question record
	EthernetUDP_write(&dns->iUdp,(uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    twoByteBuffer = 0;  // Zero answer records
	EthernetUDP_write(&dns->iUdp,(uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

	EthernetUDP_write(&dns->iUdp,(uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));
    // and zero additional records
	EthernetUDP_write(&dns->iUdp,(uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    // Build question
    const char* start =aName;
    const char* end =start;
    uint8_t len;
    // Run through the name being requested
    while (*end)
    {
        // Find out how long this section of the name is
        end = start;
        while (*end && (*end != '.') )
        {
            end++;
        }

        if (end-start > 0)
        {
            // Write out the size of this section
            len = end-start;
			EthernetUDP_write(&dns->iUdp,&len, sizeof(len));
            // And then write_byte out the section
			EthernetUDP_write(&dns->iUdp,(uint8_t*)start, end-start);
        }
        start = end+1;
    }

    // We've got to the end of the question name, so
    // terminate it with a zero-length section
    len = 0;
	EthernetUDP_write(&dns->iUdp,&len, sizeof(len));
    // Finally the type and class of question
    twoByteBuffer = htons(TYPE_A);
	EthernetUDP_write(&dns->iUdp,(uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    twoByteBuffer = htons(CLASS_IN);  // Internet class of question
	EthernetUDP_write(&dns->iUdp,(uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));
    // Success!  Everything buffered okay
    return 1;
}


uint16_t DNSClient_ProcessResponse(DNSClient* dns, uint16_t aTimeout, IPAddress* aAddress)
{
    uint32_t startTime = millis();

    // Wait for a response packet
    while(EthernetUDP_parsePacket(&dns->iUdp) <= 0)
    {
        if((millis() - startTime) > aTimeout)
            return TIMED_OUT;
        delay(50);
    }

    // We've had a reply!
    // Read the UDP header
    uint8_t header[DNS_HEADER_SIZE]; // Enough space to reuse for the DNS header
    // Check that it's a response from the right server and the right port
    if ( (dns->iDNSServer.dword != dns->iUdp._remoteIP.dword) ||
        (dns->iUdp._remotePort!= DNS_PORT) )
    {
        // It's not from who we expected
        return INVALID_SERVER;
    }

    // Read through the rest of the response
    if (EthernetUDP_available(&dns->iUdp) < DNS_HEADER_SIZE)
    {
        return TRUNCATED;
    }
	EthernetUDP_read_buf(&dns->iUdp, header, DNS_HEADER_SIZE);

    uint16_t header_flags = htons(*((uint16_t*)&header[2]));
    // Check that it's a response to this request
    if ( (dns->iRequestId != (*((uint16_t*)&header[0])) ) ||
        ((header_flags & QUERY_RESPONSE_MASK) != (uint16_t)RESPONSE_FLAG) )
    {
        // Mark the entire packet as read_string
		EthernetUDP_flush(&dns->iUdp);
        return INVALID_RESPONSE;
    }
    // Check for any errors in the response (or in our request)
    // although we don't do anything to get round these
    if ( (header_flags & TRUNCATION_FLAG) || (header_flags & RESP_MASK) )
    {
        // Mark the entire packet as read_string
		EthernetUDP_flush(&dns->iUdp);
        return -5; //INVALID_RESPONSE;
    }

    // And make sure we've got (at least) one answer
    uint16_t answerCount = htons(*((uint16_t*)&header[6]));
    if (answerCount == 0 )
    {
        // Mark the entire packet as read_string
		EthernetUDP_flush(&dns->iUdp);
        return -6; //INVALID_RESPONSE;
    }

    // Skip over any questions
    for (uint16_t i =0; i < htons(*((uint16_t*)&header[4])); i++)
    {
        // Skip over the name
        uint8_t len;
        do
        {
			EthernetUDP_read_buf(&dns->iUdp, &len, sizeof(len));
            if (len > 0)
            {
                // Don't need to actually read_string the data out for the string, just
                // advance ptr to beyond it
                while(len--)
                {
					EthernetUDP_read(&dns->iUdp ); // we don't care about the returned byte
                }
            }
        } while (len != 0);

        // Now jump over the type and class
        for (int i =0; i < 4; i++)
        {
			EthernetUDP_read(&dns->iUdp ); // we don't care about the returned byte
        }
    }

    // Now we're up to the bit we're interested in, the answer
    // There might be more than one answer (although we'll just use the first
    // type A answer) and some authority and additional resource records but
    // we're going to ignore all of them.

    for (uint16_t i =0; i < answerCount; i++)
    {
        // Skip the name
        uint8_t len;
        do
        {
			EthernetUDP_read_buf(&dns->iUdp, &len, sizeof(len));
            if ((len & LABEL_COMPRESSION_MASK) == 0)
            {
                // It's just a normal label
                if (len > 0)
                {
                    // And it's got a length
                    // Don't need to actually read_string the data out for the string,
                    // just advance ptr to beyond it
                    while(len--)
                    {
						EthernetUDP_read(&dns->iUdp ); // we don't care about the returned byte
                    }
                }
            }
            else
            {
                // This is a pointer to a somewhere else in the message for the
                // rest of the name.  We don't care about the name, and RFC1035
                // says that a name is either a sequence of labels ended with a
                // 0 length octet or a pointer or a sequence of labels ending in
                // a pointer.  Either way, when we get here we're at the end of
                // the name
                // Skip over the pointer
				EthernetUDP_read(&dns->iUdp ); // we don't care about the returned byte
                // And set len so that we drop out of the name loop
                len = 0;
            }
        } while (len != 0);

        // Check the type and class
        uint16_t answerType;
        uint16_t answerClass;
		EthernetUDP_read_buf(&dns->iUdp, (uint8_t*)&answerType, sizeof(answerType));
		EthernetUDP_read_buf(&dns->iUdp, (uint8_t*)&answerClass, sizeof(answerClass));

        // Ignore the Time-To-Live as we don't do any caching
        for (int i =0; i < TTL_SIZE; i++)
        {
			EthernetUDP_read(&dns->iUdp ); // we don't care about the returned byte
        }

        // And read_string out the length of this answer
        // Don't need header_flags anymore, so we can reuse it here
		EthernetUDP_read_buf(&dns->iUdp, (uint8_t*)&header_flags, sizeof(header_flags));

        if ( (htons(answerType) == TYPE_A) && (htons(answerClass) == CLASS_IN) )
        {
            if (htons(header_flags) != 4)
            {
                // It's a weird size
                // Mark the entire packet as read_string
				EthernetUDP_flush(&dns->iUdp);
                return -9;//INVALID_RESPONSE;
            }
			EthernetUDP_read_buf(&dns->iUdp, aAddress->bytes, 4);
			return SUCCESS;
        }
        else
        {
            // This isn't an answer type we're after, move onto the next one
            for (uint16_t i =0; i < htons(header_flags); i++)
            {
				EthernetUDP_read(&dns->iUdp ); // we don't care about the returned byte
            }
        }
    }

    // Mark the entire packet as read_string
	EthernetUDP_flush(&dns->iUdp);

    // If we get here then we haven't found an answer
    return -10;//INVALID_RESPONSE;
}

