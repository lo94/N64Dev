BUILD_DIR=build
include $(N64_INST)/include/n64.mk

OBJS = $(BUILD_DIR)/SuperSeq94.o \
	$(BUILD_DIR)/lib/ui/ui_menu.o \
	$(BUILD_DIR)/lib/util/font_util.o

assets = $(wildcard assets/*.wav)
assets_conv = $(addprefix filesystem/,$(notdir $(assets:%.wav=%.wav64))) \
			  $(addprefix filesystem/,$(notdir $(assets_ttf:%.ttf=%.font64))) \
			  $(addprefix filesystem/,$(notdir $(assets_fnt:%.fnt=%.font64))) \
              $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite))) \
			  $(addprefix filesystem/,$(notdir $(assets_txt:%.txt=%.txt)))

MKSPRITE_FLAGS ?=
MKFONT_FLAGS ?= --range all

assets_fnt = $(wildcard assets/*.fnt)
assets_ttf = $(wildcard assets/*.ttf)
assets_png = $(wildcard assets/*.png)
assets_txt = $(wildcard assets/*.txt)

all: SuperSeq94.z64

# Run audioconv on all WAV files under assets/
# We do this file by file, but we could even do it just once for the whole
# directory, because audioconv64 supports directory walking.
filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) --wav-compress 0 -o filesystem $<

filesystem/%.txt: assets/%.txt
	@mkdir -p $(dir $@)
	@cp "$<" "$@"

filesystem/%.font64: assets/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem "$<"

filesystem/%.font64: assets/%.fnt
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem "$<"

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"

filesystem/EnterCommand.font64: MKFONT_FLAGS+=--outline 1

$(BUILD_DIR)/SuperSeq94.dfs: $(assets_conv)
$(BUILD_DIR)/SuperSeq94.elf: $(OBJS)

SuperSeq94.z64: N64_ROM_TITLE="Mixer Test"
SuperSeq94.z64: $(BUILD_DIR)/SuperSeq94.dfs

clean:
	rm -f $(BUILD_DIR)/* SuperSeq94.z64 $(assets_conv)

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
