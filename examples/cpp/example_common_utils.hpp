#pragma once

#if defined(RENDERER_IMGUI)

// NOLINTNEXTLINE
#define IMGUI_COMBO(combo_items, combo_title, combo_callback)                 \
    {                                                                         \
        static size_t s_item_current_idx = 0;                                 \
        static bool s_change_item = false;                                    \
        const char* combo_preview_value = combo_items.at(s_item_current_idx); \
        if (ImGui::BeginCombo(combo_title, combo_preview_value)) {            \
            for (size_t i = 0; i < combo_items.size(); ++i) {                 \
                const auto IS_SELECTED = (s_item_current_idx == i);           \
                if (ImGui::Selectable(combo_items.at(i), IS_SELECTED)) {      \
                    s_change_item = (s_item_current_idx != i);                \
                    s_item_current_idx = i;                                   \
                }                                                             \
                if (IS_SELECTED) {                                            \
                    ImGui::SetItemDefaultFocus();                             \
                }                                                             \
            }                                                                 \
            ImGui::EndCombo();                                                \
        }                                                                     \
        if (s_change_item) {                                                  \
            combo_callback(s_item_current_idx);                               \
            s_change_item = false;                                            \
        }                                                                     \
    }

#endif
