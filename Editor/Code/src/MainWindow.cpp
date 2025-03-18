#include "MainWindow.h"

//#include "MainWindow.h"

void MainWindow::Init()
{
	
}

void MainWindow::Update()
{
	//Update here idk

	//update all subwindows
	for (auto& subWindow : m_SubWindowsMap)
	{
		subWindow.second->Update();
	}
}

void MainWindow::Destroy()
{
	

	//Destroy all subwindows
	for (auto& subWindow : m_SubWindowsMap)
	{
		subWindow.second->Destroy();
	}
	m_SubWindowsMap.clear();
}

bool MainWindow::KeyInUse(std::string key)
{
	return m_SubWindowsMap.contains(key);
}

/*
Turns the given key into a unique and valid one. Just returns the key if it is not in use.
*/
std::string MainWindow::FindValidKey(std::string key)
{
	std::string returnKey = key;
	while (KeyInUse(returnKey))
	{
		returnKey = key + std::to_string(1);
	}
	return returnKey;
}
