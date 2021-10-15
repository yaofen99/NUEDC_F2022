################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
upper_layer_control/%.obj: ../upper_layer_control/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/opt/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=5 --include_path="/home/hq/workspace_v10/project" --include_path="/home/hq/workspace_v10/project/Debug__opt_for_speed__5" --include_path="/opt/ti/ccs1040/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/opt/ti/ccs1040/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="/opt/ti/ccs1040/simplelink_msp432p4_sdk_3_40_01_02/source/ti/posix/ccs" --include_path="/opt/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="upper_layer_control/$(basename $(<F)).d_raw" --include_path="/home/hq/workspace_v10/project/Debug__opt_for_speed__5/syscfg" --obj_directory="upper_layer_control" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


