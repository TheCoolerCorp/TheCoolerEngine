#pragma once
#include <string>
#include <unordered_map>

#include "SubWindow.h"

class MainWindow
{
public:
	void Init();
	void Update();
	void Destroy();

	template<typename T>
	SubWindow* CreateWindow(std::string name)
	{
		static_assert(std::is_base_of<SubWindow, T>::value, "MainWindow:CreateWindow : T must be a SubWindow!");
		auto subWindow = std::make_shared<T>();
		subWindow->Init();
		m_SubWindows.push_back(subWindow);
		return subWindow.get();
	}
private:
	bool KeyInUse(std::string key);
	std::string FindValidKey(std::string key);

	std::unordered_map<std::string, std::shared_ptr<SubWindow>> m_SubWindowsMap;
};
