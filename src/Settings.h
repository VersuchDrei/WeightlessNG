#pragma once

#include "AutoTOML.hpp"

class Settings {
public:
	using ISetting = AutoTOML::ISetting;
    using bSetting = AutoTOML::bSetting;

    static void load() {
        try {
            const auto table = toml::parse_file("Data/SKSE/Plugins/Weightless.toml"s);
            for (const auto& setting : ISetting::get_settings()) {
                setting->load(table);
            }
        } catch (const toml::parse_error& e) {
            std::ostringstream ss;
            ss << "Error parsing file \'" << *e.source().path << "\':\n"
               << '\t' << e.description() << '\n'
               << "\t\t(" << e.source().begin << ')';
            logger::error("{}"sv, ss.str());
            throw std::runtime_error("failed to load settings"s);
        }
    }

    static inline bSetting books{"General"s, "books"s, true};
    static inline bSetting soulgems{"General"s, "soulgems"s, true};
    static inline bSetting food{"General"s, "food"s, true};
    static inline bSetting ingredients{"General"s, "ingredients"s, true};
    static inline bSetting potions{"General"s, "potions"s, true};
    static inline bSetting scrolls{"General"s, "scrolls"s, true};
    static inline bSetting gems{"General"s, "gems"s, true};
    static inline bSetting ingotsandores{"General"s, "ingotsandores"s, true};
    static inline bSetting animalparts{"General"s, "animalparts"s, true};
    static inline bSetting clutter{"General"s, "clutter"s, true};
    static inline bSetting misc{"General"s, "misc"s, true};
};