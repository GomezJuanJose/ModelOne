#pragma once

#include <memory>

/*#ifdef MO_PLATFORM_WINDOWS
	#ifdef MO_BUILD_DLL
		#define ModelOne_API __declspec(dllexport)
	#else
		#define ModelOne_API __declspec(dllimport)
	#endif
#else
	#error ModelOne at the moment only supports Windows!
#endif
*/

#ifdef NDEBUG
	#define MO_ENABLE_ASSERTS 0
#else
	#define MO_ENABLE_ASSERTS 1
#endif // NDEBUG



#ifdef MO_ENABLE_ASSERTS
	#define MO_ASSERT(condition, message, ...) {if(!(condition)){MO_LOG_ERROR(true, message, __VA_ARGS__); __debugbreak();}}
#else
	#define MO_ASSERT(condition, message, ...)
#endif

#define BIT(x) (1 << x)

#define MO_PROFILE 1
#define MO_PROFILE_RENDERER 1

namespace ModelOne {
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