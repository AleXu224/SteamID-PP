#include "include/SteamID.hpp"
#include <string_view>
#include <cassert>

int main() {
    {
        constexpr auto id = SteamID();
        static_assert(id.universe == SteamID::Universe::INVALID);
        static_assert(id.type == SteamID::Type::INVALID);
        static_assert(id.instance == SteamID::Instance::ALL);
        static_assert(id.accountID == 0);
    }
    {
		constexpr auto id = SteamID::fromIndividualAccountID(46143802);
        static_assert(id.isValid());
        static_assert(id.isValidIndividual());
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::INDIVIDUAL);
        static_assert(id.instance == SteamID::Instance::DESKTOP);
        static_assert(id.accountID == 46143802);
	}
    {
        constexpr auto id = SteamID::fromIndividualAccountID(0);
        static_assert(!id.isValid());
    }
    {
		constexpr auto id = SteamID("STEAM_0:0:23071901");
		static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::INDIVIDUAL);
        static_assert(id.instance == SteamID::Instance::DESKTOP);
        static_assert(id.accountID == 46143802);
    }
    {
		constexpr auto id = SteamID("STEAM_1:1:23071901");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::INDIVIDUAL);
        static_assert(id.instance == SteamID::Instance::DESKTOP);
        static_assert(id.accountID == 46143803);
	}
    {
		constexpr auto id = SteamID("[U:1:46143802]");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::INDIVIDUAL);
        static_assert(id.instance == SteamID::Instance::DESKTOP);
        static_assert(id.accountID == 46143802);
	}
    {
        constexpr auto id = SteamID("[G:1:31]");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::GAMESERVER);
        static_assert(id.instance == SteamID::Instance::ALL);
        static_assert(id.accountID == 31);
        static_assert(id.isValid());
        static_assert(!id.isValidIndividual());
    }
    {
		constexpr auto id = SteamID("[A:1:46124:11245]");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::ANON_GAMESERVER);
        static_assert(id.instance == 11245);
        static_assert(id.accountID == 46124);
	}
    {
		constexpr auto id = SteamID("[L:1:12345]");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::CHAT);
        static_assert(id.instance == SteamID::ChatInstanceFlags::Lobby);
        static_assert(id.accountID == 12345);
	}
    {
		constexpr auto id = SteamID("[L:1:12345:55]");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::CHAT);
        static_assert(id.instance == (SteamID::ChatInstanceFlags::Lobby | 55));
        static_assert(id.accountID == 12345);
	}
    {
		constexpr auto id = SteamID("76561198006409530");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::INDIVIDUAL);
        static_assert(id.instance == SteamID::Instance::DESKTOP);
        static_assert(id.accountID == 46143802);
	}
    {
		constexpr auto id = SteamID("103582791434202956");
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::CLAN);
        static_assert(id.instance == SteamID::Instance::ALL);
		static_assert(id.accountID == 4681548);
	}
    {
		constexpr auto id = SteamID(76561198006409530);
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::INDIVIDUAL);
        static_assert(id.instance == SteamID::Instance::DESKTOP);
        static_assert(id.accountID == 46143802);
	}
    {
        constexpr auto id = SteamID(103582791434202956);
        static_assert(id.universe == SteamID::Universe::PUBLIC);
        static_assert(id.type == SteamID::Type::CLAN);
        static_assert(id.instance == SteamID::Instance::ALL);
        static_assert(id.accountID == 4681548);
    }
    {
		constexpr auto id = SteamID(76561198006409530);
		static_assert(id.getSteam2RenderedID() == "STEAM_0:0:23071901");
        static_assert(id.steam2() == "STEAM_0:0:23071901");
    }
    {
		constexpr auto id = SteamID("STEAM_1:0:23071901");
        static_assert(id.getSteam2RenderedID(true) == "STEAM_1:0:23071901");
	}
    {
		constexpr auto id = SteamID(76561198006409530);
        static_assert(id.getSteam3RenderedID() == "[U:1:46143802]");
        static_assert(id.steam3() == "[U:1:46143802]");
	}
    {
		constexpr auto id = SteamID("[A:1:43253156:41511]");
        static_assert(id.getSteam3RenderedID() == "[A:1:43253156:41511]");
	}
    {
		constexpr auto id = SteamID("[L:1:451932]");
        static_assert(id.getSteam3RenderedID() == "[L:1:451932]");
	}
    {
		constexpr auto id = SteamID("76561198006409530");
		static_assert(id.getSteamID64() == "76561198006409530");
		static_assert(id.getIntID() == 76561198006409530);
	}
	{
		constexpr auto id = SteamID("90883702753783269");
        static_assert(id.getSteamID64() == "90883702753783269");
        static_assert(id.getIntID() == 90883702753783269);
	}
    {
        constexpr auto id = SteamID();
        static_assert(!id.isValid());
    }
    {
		constexpr auto id = SteamID("[U:1:46143802:10]");
        static_assert(!id.isValid());
	}
    {
		constexpr auto id = SteamID("[g:1:4681548:2]");
        static_assert(!id.isValid());
	}
    {
		constexpr auto id = SteamID("[G:1:0]");
        static_assert(!id.isValid());
	}
}