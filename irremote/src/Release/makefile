################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
ir-tester.lss \

FLASH_IMAGE += \
ir-tester.hex \

EEPROM_IMAGE += \
ir-tester.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ir-tester.elf secondary-outputs

# Tool invocations
ir-tester.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ir-tester.map -mmcu=atmega8 -o"ir-tester.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ir-tester.lss: ir-tester.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ir-tester.elf  >"ir-tester.lss"
	@echo 'Finished building: $@'
	@echo ' '

ir-tester.hex: ir-tester.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex ir-tester.elf  "ir-tester.hex"
	@echo 'Finished building: $@'
	@echo ' '

ir-tester.eep: ir-tester.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ir-tester.elf  "ir-tester.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ir-tester.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t ir-tester.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) ir-tester.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
