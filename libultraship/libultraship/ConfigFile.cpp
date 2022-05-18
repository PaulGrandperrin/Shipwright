#include "ConfigFile.h"
#include "spdlog/spdlog.h"
#include "GlobalCtx2.h"
#include "Window.h"
#include "GameSettings.h"
#include <filesystem>

namespace Ship {
	ConfigFile::ConfigFile(std::shared_ptr<GlobalCtx2> Context) : Context(Context), File("") {
		std::filesystem::path conf_path;
		if(const char* xdg_conf_home = std::getenv("XDG_CONFIG_HOME")) {
			conf_path += xdg_conf_home;
			conf_path += "/shipwright";
		} else if (const char* home = std::getenv("HOME")) {
			conf_path += home;
			conf_path += "/.config/shipwright";
		} else {
			conf_path += ".";
		}

		if (!(std::filesystem::exists(conf_path) || std::filesystem::create_directories(conf_path))) {
			SPDLOG_ERROR("Failed to create directories for default configs");
			exit(EXIT_FAILURE);
		}

		conf_path += "/shipofharkinian.ini";

		Path = conf_path;
		File = mINI::INIFile(Path.c_str());

		if (!File.read(Val)) {
			if (!CreateDefaultConfig()) {
				SPDLOG_ERROR("Failed to create default configs");
				exit(EXIT_FAILURE);
			}
		}
	}

	ConfigFile::~ConfigFile() {
		if (!Save()) {
			SPDLOG_ERROR("Failed to save configs!!!");
		}

		SPDLOG_INFO("destruct configfile");
	}

	mINI::INIMap<std::string>& ConfigFile::operator[](const std::string& Section) {
		return Val[Section];
	}

	mINI::INIMap<std::string> ConfigFile::get(const std::string& Section) {
		return Val.get(Section);
	}

	bool ConfigFile::has(const std::string& Section) {
		return Val.has(Section);
	}

	bool ConfigFile::remove(const std::string& Section) {
		return Val.remove(Section);
	}

	void ConfigFile::clear() {
		Val.clear();
	}

	std::size_t ConfigFile::size() const {
		return Val.size();
	}

	bool ConfigFile::Save() {
		return File.write(Val);
	}

	bool ConfigFile::CreateDefaultConfig() {
		(*this)["ARCHIVE"]["Main Archive"] = "oot.otr";
		(*this)["ARCHIVE"]["Patches Directory"] = "";

		(*this)["CONTROLLERS"]["CONTROLLER 1"] = "Auto";
		(*this)["CONTROLLERS"]["CONTROLLER 2"] = "Unplugged";
		(*this)["CONTROLLERS"]["CONTROLLER 3"] = "Unplugged";
		(*this)["CONTROLLERS"]["CONTROLLER 4"] = "Unplugged";

		(*this)["KEYBOARD SHORTCUTS"]["KEY_FULLSCREEN"] = std::to_string(0x044);
		(*this)["KEYBOARD SHORTCUTS"]["KEY_CONSOLE"] = std::to_string(0x029);

		(*this)["WINDOW"]["WINDOW WIDTH"] = std::to_string(320);
		(*this)["WINDOW"]["WINDOW HEIGHT"] = std::to_string(240);
		(*this)["WINDOW"]["FULLSCREEN WIDTH"] = std::to_string(1920);
		(*this)["WINDOW"]["FULLSCREEN HEIGHT"] = std::to_string(1080);
		(*this)["WINDOW"]["FULLSCREEN"] = std::to_string(false);
		(*this)["WINDOW"]["GFX BACKEND"] = "";

		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_CRIGHT)] = std::to_string(0x14D);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_CLEFT)] = std::to_string(0x14B);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_CDOWN)] = std::to_string(0x150);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_CUP)] = std::to_string(0x148);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_R)] = std::to_string(0x013);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_L)] = std::to_string(0x012);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_DRIGHT)] = std::to_string(0x023);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_DLEFT)] = std::to_string(0x021);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_DDOWN)] = std::to_string(0x022);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_DUP)] = std::to_string(0x014);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_START)] = std::to_string(0x039);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_Z)] = std::to_string(0x02C);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_B)] = std::to_string(0x02E);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_A)] = std::to_string(0x02D);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_STICKRIGHT)] = std::to_string(0x020);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_STICKLEFT)] = std::to_string(0x01E);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_STICKDOWN)] = std::to_string(0x01F);
		(*this)["KEYBOARD CONTROLLER BINDING 1"][STR(BTN_STICKUP)] = std::to_string(0x011);

		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_CRIGHT)] = std::to_string(0x14D);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_CLEFT)] = std::to_string(0x14B);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_CDOWN)] = std::to_string(0x150);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_CUP)] = std::to_string(0x148);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_R)] = std::to_string(0x013);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_L)] = std::to_string(0x012);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_DRIGHT)] = std::to_string(0x023);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_DLEFT)] = std::to_string(0x021);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_DDOWN)] = std::to_string(0x022);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_DUP)] = std::to_string(0x014);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_START)] = std::to_string(0x039);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_Z)] = std::to_string(0x02C);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_B)] = std::to_string(0x02E);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_A)] = std::to_string(0x02D);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_STICKRIGHT)] = std::to_string(0x020);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_STICKLEFT)] = std::to_string(0x01E);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_STICKDOWN)] = std::to_string(0x01F);
		(*this)["KEYBOARD CONTROLLER BINDING 2"][STR(BTN_STICKUP)] = std::to_string(0x011);

		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_CRIGHT)] = std::to_string(0x14D);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_CLEFT)] = std::to_string(0x14B);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_CDOWN)] = std::to_string(0x150);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_CUP)] = std::to_string(0x148);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_R)] = std::to_string(0x013);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_L)] = std::to_string(0x012);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_DRIGHT)] = std::to_string(0x023);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_DLEFT)] = std::to_string(0x021);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_DDOWN)] = std::to_string(0x022);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_DUP)] = std::to_string(0x014);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_START)] = std::to_string(0x039);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_Z)] = std::to_string(0x02C);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_B)] = std::to_string(0x02E);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_A)] = std::to_string(0x02D);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_STICKRIGHT)] = std::to_string(0x020);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_STICKLEFT)] = std::to_string(0x01E);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_STICKDOWN)] = std::to_string(0x01F);
		(*this)["KEYBOARD CONTROLLER BINDING 3"][STR(BTN_STICKUP)] = std::to_string(0x011);

		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_CRIGHT)] = std::to_string(0x14D);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_CLEFT)] = std::to_string(0x14B);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_CDOWN)] = std::to_string(0x150);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_CUP)] = std::to_string(0x148);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_R)] = std::to_string(0x013);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_L)] = std::to_string(0x012);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_DRIGHT)] = std::to_string(0x023);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_DLEFT)] = std::to_string(0x021);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_DDOWN)] = std::to_string(0x022);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_DUP)] = std::to_string(0x014);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_START)] = std::to_string(0x039);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_Z)] = std::to_string(0x02C);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_B)] = std::to_string(0x02E);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_A)] = std::to_string(0x02D);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_STICKRIGHT)] = std::to_string(0x020);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_STICKLEFT)] = std::to_string(0x01E);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_STICKDOWN)] = std::to_string(0x01F);
		(*this)["KEYBOARD CONTROLLER BINDING 4"][STR(BTN_STICKUP)] = std::to_string(0x011);

		(*this)["ENHANCEMENT SETTINGS"]["TEXT_SPEED"] = "1";

		(*this)["SDL CONTROLLER 1"]["GUID"] = "";
		(*this)["SDL CONTROLLER 2"]["GUID"] = "";
		(*this)["SDL CONTROLLER 3"]["GUID"] = "";
		(*this)["SDL CONTROLLER 4"]["GUID"] = "";

		return File.generate(Val);
	}
}
