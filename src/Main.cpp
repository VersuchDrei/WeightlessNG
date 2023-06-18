#include <stddef.h>

#include "Settings.h"

using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace {
    void InitializeLogging() {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }
        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    }

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kDataLoaded: {
                RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();

                if (*Settings::books) {
                    auto& books = handler->GetFormArray<RE::TESObjectBOOK>();
                    for (RE::TESObjectBOOK*& book : books) {
                        book->weight = 0;
                    }
                }

                if (*Settings::soulgems) {
                    auto& soulgems = handler->GetFormArray<RE::TESSoulGem>();
                    for (RE::TESSoulGem*& soulgem : soulgems) {
                        soulgem->weight = 0;
                    }
                }

                if (*Settings::ingredients) {
                    auto& ingredients = handler->GetFormArray<RE::IngredientItem>();
                    for (RE::IngredientItem*& ingredient : ingredients) {
                        ingredient->weight = 0;
                    }
                }

                if (*Settings::food || *Settings::potions) {
                    RE::BGSKeyword* VendorItemFood = handler->LookupForm<RE::BGSKeyword>(0x08CDEA, "Skyrim.esm");
                    RE::BGSKeyword* VendorItemFoodRaw = handler->LookupForm<RE::BGSKeyword>(0x0A0E56, "Skyrim.esm");

                    auto& potions = handler->GetFormArray<RE::AlchemyItem>();
                    for (RE::AlchemyItem*& potion : potions) {
                        if (potion->HasKeyword(VendorItemFood) || potion->HasKeyword(VendorItemFoodRaw)) {
                            if (*Settings::food) {
                                potion->weight = 0;
                            }
                        } else {
                            if (*Settings::potions) {
                                potion->weight = 0;
                            }
                        }
                    }
                }
                
                if (*Settings::scrolls) {
                    auto& scrolls = handler->GetFormArray<RE::ScrollItem>();
                    for (RE::ScrollItem*& scroll : scrolls) {
                        scroll->weight = 0;
                    }
                }
                
                if (*Settings::gems || *Settings::ingotsandores || *Settings::animalparts || *Settings::clutter || *Settings::misc) {
                    RE::BGSKeyword* VendorItemGem = handler->LookupForm<RE::BGSKeyword>(0x0914ED, "Skyrim.esm");
                    RE::BGSKeyword* VendorItemOreIngot = handler->LookupForm<RE::BGSKeyword>(0x0914EC, "Skyrim.esm");
                    RE::BGSKeyword* VendorItemAnimalHide = handler->LookupForm<RE::BGSKeyword>(0x0914EA, "Skyrim.esm");
                    RE::BGSKeyword* VendorItemAnimalPart = handler->LookupForm<RE::BGSKeyword>(0x0914EB, "Skyrim.esm");
                    RE::BGSKeyword* VendorItemClutter = handler->LookupForm<RE::BGSKeyword>(0x0914E9, "Skyrim.esm");

                    auto& misc = handler->GetFormArray<RE::TESObjectMISC>();
                    for (RE::TESObjectMISC*& item : misc) {
                        if (item->HasKeyword(VendorItemGem)) {
                            if (*Settings::gems) {
                                item->weight = 0;
                            }
                        } else if (item->HasKeyword(VendorItemOreIngot)) {
                            if (*Settings::ingotsandores) {
                                item->weight = 0;
                            }
                        } else if (item->HasKeyword(VendorItemAnimalHide) || item->HasKeyword(VendorItemAnimalPart)) {
                            if (*Settings::animalparts) {
                                item->weight = 0;
                            }
                        } else if (item->HasKeyword(VendorItemClutter)) {
                            if (*Settings::clutter) {
                                item->weight = 0;
                            }
                        } else {
                            if (*Settings::misc) {
                                item->weight = 0;
                            }
                        }
                    }
                }
            } break;
        }
    }
}

SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);

    Settings::load();

    auto message = SKSE::GetMessagingInterface();
    if (!message->RegisterListener(MessageHandler)) {
        return false;
    }

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
