#pragma once
#include <cstdint>
#include <format>
#include <string_view>
#include "string"

struct SteamID {
    enum class Universe : uint64_t {
        INVALID = 0,
        PUBLIC = 1,
        BETA = 2,
        INTERNAL = 3,
        DEV = 4,
    };

    enum class Type :uint64_t {
        INVALID = 0,
        INDIVIDUAL = 1,
        MULTISEAT = 2,
        GAMESERVER = 3,
        ANON_GAMESERVER = 4,
        PENDING = 5,
        CONTENT_SERVER = 6,
        CLAN = 7,
        CHAT = 8,
        P2P_SUPER_SEEDER = 9,
        ANON_USER = 10,
    };

    struct Instance {
        static constexpr uint64_t ALL = 0;
        static constexpr uint64_t DESKTOP = 1;
        static constexpr uint64_t CONSOLE = 2;
        static constexpr uint64_t WEB = 4;

        Instance() = delete;
    };

    struct Masks {
        static constexpr uint64_t AccountIDMask = 0xFFFFFFFF;
        static constexpr uint64_t AccountInstanceMask = 0x000FFFFF;

        Masks() = delete;
    };

    struct ChatInstanceFlags {
        static constexpr uint64_t Clan = (Masks::AccountIDMask + 1) >> 1;
        static constexpr uint64_t Lobby = (Masks::AccountIDMask + 1) >> 2;
        static constexpr uint64_t MMSLobby = (Masks::AccountInstanceMask + 1) >> 3;

        ChatInstanceFlags() = delete;
    };

	static constexpr char TypeChar(const SteamID::Type &type) {
		switch (type) {
			case SteamID::Type::INVALID:
				return 'I';
			case SteamID::Type::INDIVIDUAL:
				return 'U';
			case SteamID::Type::MULTISEAT:
				return 'M';
			case SteamID::Type::GAMESERVER:
				return 'G';
			case SteamID::Type::ANON_GAMESERVER:
				return 'A';
			case SteamID::Type::PENDING:
				return 'P';
			case SteamID::Type::CONTENT_SERVER:
				return 'C';
			case SteamID::Type::CLAN:
				return 'g';
			case SteamID::Type::CHAT:
				return 'T';
			// case SteamID::Type::P2P_SUPER_SEEDER: // Not used
			// 	return '';
			case SteamID::Type::ANON_USER:
				return 'a';
			default:
				return '?';
		}
	}

	static constexpr SteamID::Type CharType(const char &type) {
		switch (type) {
			case 'I':
				return SteamID::Type::INVALID;
			case 'U':
				return SteamID::Type::INDIVIDUAL;
			case 'M':
				return SteamID::Type::MULTISEAT;
			case 'G':
				return SteamID::Type::GAMESERVER;
			case 'A':
				return SteamID::Type::ANON_GAMESERVER;
			case 'P':
				return SteamID::Type::PENDING;
			case 'C':
				return SteamID::Type::CONTENT_SERVER;
			case 'g':
				return SteamID::Type::CLAN;
			case 'T':
				return SteamID::Type::CHAT;
			// case SteamID::Type::P2P_SUPER_SEEDER: // Not used
			// 	return '';
			case 'a':
				return SteamID::Type::ANON_USER;
			default:
				return SteamID::Type::INVALID;
		}
	}

	Universe universe = Universe::INVALID;
    Type type = Type::INVALID;
    uint64_t instance = Instance::ALL;
    uint64_t accountID = 0;

    constexpr SteamID() = default;
	constexpr SteamID(std::string_view id) {
		// A SteamID3 can't have less than 7 characters
		if (id.size() < 7) return;

        // SteamID3
		if (id.front() == '[' && id.back() == ']') {
			// [U:1:182986314]
			if (id[1] < 'A' || id[1] > 'z') return;
			if (id[2] != ':') return;
			if (id[3] < '0' || id[3] > '5') return;
			if (id[4] != ':') return;
			bool hasInstance = false;
			for (const auto &c: id.substr(5, id.size() - 6)) {
				if (c == ':') {
					hasInstance = true;
					continue;
				}
				if (c < '0' || c > '9') return;
			}

			universe = static_cast<Universe>(stoi(std::string(id.substr(3, 1))));
			if (!hasInstance) {
				accountID = stoi(std::string(id.substr(5, id.size() - 6)));
			} else {
				const auto subStr = id.substr(5, id.size() - 6);
				accountID = stoi(std::string(subStr.substr(0, subStr.find(':'))));
				instance = stoi(std::string(subStr.substr(subStr.find(':') + 1)));
			}

			switch (id[1]) {
				case 'U': {
					type = Type::INDIVIDUAL;
					if (!hasInstance) instance = Instance::DESKTOP;
					break;
				}
				case 'c': {
					type = Type::CHAT;
					instance |= ChatInstanceFlags::Clan;
					break;
				}
				case 'L': {
					type = Type::CHAT;
					instance |= ChatInstanceFlags::Lobby;
					break;
				}
				default: {
					type = CharType(id[1]);
				}
			}

			return;
		}

		if (id.size() < 11) return;

		// SteamID2
		if (id.substr(0, 6).compare("STEAM_") == 0) {
            if (id[6] < '0' || id[6] > '5') return;
            if (id[7] != ':') return;
            if (id[8] < '0' || id[8] > '1') return;
            if (id[9] != ':') return;
            for (const auto &c: id.substr(10)) {
                if (c < '0' || c > '9') return;
            }

            universe = static_cast<Universe>(stoi(std::string(id.substr(6, 1))));
            if (universe == Universe::INVALID) universe = Universe::PUBLIC;
            type = Type::INDIVIDUAL;
            instance = Instance::DESKTOP;
            accountID = (stoi(std::string(id.substr(10))) << 1) + stoi(std::string(id.substr(8, 1)));
        }

        // SteamID64
        for (const auto &c: id) {
            if (c < '0' || c > '9') return;
        }
        uint64_t idInt = stoi(std::string(id));
        universe = static_cast<Universe>(idInt >> 56);
        type = static_cast<Type>((idInt >> 52) & 0xF);
        instance = (idInt >> 32) & Masks::AccountInstanceMask;
        accountID = idInt & Masks::AccountIDMask;
	}

	constexpr SteamID(uint64_t id) : universe(static_cast<Universe>(id >> 56)),
									 type(static_cast<Type>((id >> 52) & 0xF)),
									 instance((id >> 32) & Masks::AccountInstanceMask),
									 accountID(id & Masks::AccountIDMask) {}

	static constexpr SteamID fromIndividualAccountID(const uint64_t &id) {
		SteamID steamID{};
		steamID.universe = Universe::PUBLIC;
		steamID.type = Type::INDIVIDUAL;
		steamID.instance = Instance::DESKTOP;
		steamID.accountID = id;
		return steamID;
	}

	[[nodiscard]] constexpr bool isValid() const {
		if (type <= Type::INVALID || type > Type::ANON_USER) return false;
		if (universe <= Universe::INVALID || universe > Universe::DEV) return false;
		if (type == Type::INDIVIDUAL && (accountID == 0 || instance > Instance::WEB)) return false;
		if (type == Type::CLAN && (accountID == 0 || instance != Instance::ALL)) return false;
		if (type == Type::GAMESERVER && accountID == 0) return false;

		return true;
	}
	[[nodiscard]] constexpr bool isValidIndividual() const {
		return universe == Universe::PUBLIC && type == Type::INDIVIDUAL && instance == Instance::DESKTOP && isValid();
	}
	[[nodiscard]] constexpr bool isGroupChat() const {
		return type == Type::CHAT && (instance & ChatInstanceFlags::Clan) != 0;
	}
	[[nodiscard]] constexpr bool isLobby() const {
		return type == Type::CHAT && ((instance & ChatInstanceFlags::Lobby) != 0 || (instance & ChatInstanceFlags::MMSLobby) != 0);
	}

	[[nodiscard]] constexpr std::string steam2(const bool &newerFormat = false) const {
		if (type != Type::INDIVIDUAL) throw std::runtime_error("Can't get Steam2 rendered ID from non-individual ID");

		const Universe usedUniverse = (!newerFormat && this->universe == Universe::PUBLIC) ? Universe::INVALID : this->universe;

        // Sadly std::format isn't constexpr yet
        std::string ret = "STEAM_";
        ret += constexpr_to_string(static_cast<uint64_t>(usedUniverse));
        ret += ':';
        ret += constexpr_to_string(accountID & 1);
        ret += ':';
        ret += constexpr_to_string(accountID >> 1);

        return ret;
	}
	[[nodiscard]] constexpr std::string getSteam2RenderedID(const bool &newerFormat = false) const {
		return steam2(newerFormat);
	}

	[[nodiscard]] constexpr std::string steam3() const {
		const auto typeChar = [&]() -> char {
			auto ret = TypeChar(type);
			if (ret == '?') ret = 'i';

			if (instance & ChatInstanceFlags::Clan) ret = 'c';
			else if (instance & ChatInstanceFlags::Lobby)
				ret = 'L';

			return ret;
		}();

		bool shouldRenderInstance =
			type == Type::ANON_GAMESERVER ||
			type == Type::MULTISEAT ||
			(type == Type::INDIVIDUAL && instance != Instance::DESKTOP);

		// return std::format("[{}:{}:{}{}]",
		// 				   typeChar,
		// 				   static_cast<uint64_t>(universe),
		// 				   accountID,
		// 				   shouldRenderInstance ? std::format(":{}", instance) : "");
        std::string ret = "[";
        ret += typeChar;
        ret += ":";
        ret += constexpr_to_string(static_cast<uint64_t>(universe));
        ret += ":";
        ret += constexpr_to_string(accountID);
        if (shouldRenderInstance) {
            ret += ":";
            ret += constexpr_to_string(instance);
        }
        ret += "]";
        return ret;
	}
	[[nodiscard]] constexpr std::string getSteam3RenderedID() const {
		return steam3();
	}

	[[nodiscard]] constexpr uint64_t getIntID() const {
		return (
			static_cast<uint64_t>(universe) << 56 |
			static_cast<uint64_t>(type) << 52 |
			static_cast<uint64_t>(instance) << 32 |
			accountID);
	}

	[[nodiscard]] constexpr std::string getSteamID64() const {
		return constexpr_to_string(getIntID());
	}

	[[nodiscard]] constexpr bool operator==(const SteamID &other) const {
		return universe == other.universe && type == other.type && instance == other.instance && accountID == other.accountID;
	}

private:
    static constexpr uint64_t stoi(std::string_view num) {
        uint64_t ret = 0;
        for (const auto &c: num) {
            if (c < '0' || c > '9') return ret;
            ret = ret * 10 + (c - '0');
        }
        return ret;
    }

    static constexpr std::string constexpr_to_string(uint64_t num) {
        if (num == 0) return "0";
        std::string ret{};
        while (num) {
            ret += static_cast<char>('0' + static_cast<uint32_t>(num % 10));
            num /= 10;
        }
        std::reverse(ret.begin(), ret.end());
        return ret;
    }
};