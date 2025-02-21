#pragma once

#include <memory>

/*#ifdef TL_PLATFORM_WINDOWS
	#ifdef TL_BUILD_DLL
		#define Primitivo_API __declspec(dllexport)
	#else
		#define Primitivo_API __declspec(dllimport)
	#endif
#else
	#error Primitivo at the moment only supports Windows!
#endif
*/

#ifdef NDEBUG
	#define TL_ENABLE_ASSERTS 0
#else
	#define TL_ENABLE_ASSERTS 1
#endif // NDEBUG



#ifdef TL_ENABLE_ASSERTS
	#define TL_ASSERT(condition, message, ...) {if(!(condition)){TL_LOG_ERROR(true, message, __VA_ARGS__); __debugbreak();}}
#else
	#define TL_ASSERT(condition, message, ...)
#endif

#define BIT(x) (1 << x)

#define TL_PROFILE 1
#define TL_PROFILE_RENDERER 1

namespace Primitivo {
	template<typename T>
	using AssetScope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr AssetScope<T> CreateAssetScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using AssetRef = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr AssetRef<T> CreateAssetRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}