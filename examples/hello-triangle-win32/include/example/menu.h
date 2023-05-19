#pragma once
#include <wil/resource.h>

namespace UT
{
	class Menu
	{
	public:
		enum class Mode
		{
			Image,
			Video,
			Model,
			Game
		};
	public:
		Menu() = default;
	public:
		void Initialize(HMENU menu)noexcept
		{
			main_menu.reset(menu);
			file.reset(GetSubMenu(menu, 0));
			view.reset(GetSubMenu(menu, 1));
			mode.reset(GetSubMenu(menu, 2));
			styles.reset(GetSubMenu(view.get(), 1));
		}
		void EnableLoading()noexcept
		{
			EnableMenuItem(file.get(), 0, MF_BYPOSITION | MF_ENABLED);
		}
		void DisableLoading()noexcept
		{
			EnableMenuItem(file.get(), 0, MF_BYPOSITION | MF_DISABLED);
		}
		void ToggleGrid()noexcept
		{
			grid_enabled ^= 1;
			MENUITEMINFO mii
			{
				.cbSize = sizeof(MENUITEMINFO),
				.fMask = MIIM_STATE,
				.fState = UINT(grid_enabled ? MFS_CHECKED : MFS_UNCHECKED)
			};
			SetMenuItemInfo(view.get(), 0, true, &mii);
		}
		bool GridEnabled()const noexcept { return grid_enabled; }
	private:
		wil::unique_hmenu main_menu;
		wil::unique_hmenu file;
		wil::unique_hmenu view;
		wil::unique_hmenu styles;
		wil::unique_hmenu mode;

		bool grid_enabled = false;
	};
}