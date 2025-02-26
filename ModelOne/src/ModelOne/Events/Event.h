#pragma once

#include "ModelOne/Core/Core.h"


namespace ModelOne {

	//TODO
	//Events on ModelOne are currently blocking, meaning when an event occurs it
	//immediately gets distpached and must be dealt with right.
	//For the future, a better strategy might be to buffer events in an event 
	//bus and process them during the "event" part of the update stage

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\
								   virtual EventType GetEventType() const override {return GetStaticType();}\
								   virtual const char* GetName() const override {return #type;}

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class Event {
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0; //TODO Should be only used on debug builds and trimmed for shipping?
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

	public:
		//This is for avoid propagation to further layer, 
		//for example if the click event is being handle by the UI then its no use to propagate 
		//that event to the World itself because that event has fall on the bounds of the UI
		bool Handled = false; //TODO think to rename it to IsHandle depending of the coding guidelines used
	};



	class EventDispatcher {
		template <typename T>
		using EventFn = std::function<bool (T&)> ;
	
	public:
		EventDispatcher(Event& event) : m_Event(event){}

		template <typename T>
		bool Dispatch(EventFn<T> func) {//TODO Put a safety check to see if is an Event type
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};
}