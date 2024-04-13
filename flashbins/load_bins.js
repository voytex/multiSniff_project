importPackage(Packages.com.ti.debug.engine.scripting);
importPackage(Packages.com.ti.ccstudio.scripting.environment);
importPackage(Packages.java.lang);

var env = ScriptingEnvironment.instance();
var server = env.getServer("DebugServer.1");
var session = server.openSession(
  "Texas Instruments XDS110 USB Debug Probe/Cortex_M4_0"
);

var index_path =
  "C:\\Users\\Administrator\\workspace_v12\\multiSniff_LP_CC2652RB_tirtos7_ticlang2\\flashbins\\index.bin";
var html_memory_location = 0x51000;

var stv_path =
  "C:\\Users\\Administrator\\workspace_v12\\multiSniff_LP_CC2652RB_tirtos7_ticlang2\\flashbins\\status_vectors.bin";
var stv_memory_location = 0x50000;

session.memory.loadBinaryProgram(index_path, html_memory_location);
session.memory.loadBinaryProgram(stv_path, stv_memory_location);
