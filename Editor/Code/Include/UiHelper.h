#ifndef UIHELPER_H
#define UIHELPER_H
#include "Math/vec3.h"


/*
 * static Ui Helper class for quickly creating various Ui elements.
 * In no namespace for quick and convenient acess.
 */
class UiHelper
{
public:
	static int UiAddObjectDragDropTarget(const char* a_text, int a_id);
	static bool UiAddObjectDragDropTarget(const char* a_text, int a_id, int& a_outId);
	static bool UiDisplayVec3(std::string a_label, int a_id, Engine::Math::vec3& a_vec3, float a_speed = 0.1f, float a_min = -FLT_MAX, float a_max = +FLT_MAX);
};

#endif // !UIHELPER_H