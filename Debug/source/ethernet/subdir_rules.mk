################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/ethernet/%.o: ../source/ethernet/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-armllvm_2.1.2.LTS/bin/tiarmclang.exe" -c -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -mthumb -Oz -I"C:/Users/Administrator/workspace_v12/multiSniff_LP_CC2652RB_tirtos7_ticlang2" -I"C:/Users/Administrator/workspace_v12/multiSniff_LP_CC2652RB_tirtos7_ticlang2/Debug" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_6_30_01_03/source" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_6_30_01_03/kernel/tirtos7/packages" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_6_30_01_03/source/ti/posix/ticlang" -DDeviceFamily_CC26X2 -gdwarf-3 -march=armv7e-m -MMD -MP -MF"source/ethernet/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/Administrator/workspace_v12/multiSniff_LP_CC2652RB_tirtos7_ticlang2/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


