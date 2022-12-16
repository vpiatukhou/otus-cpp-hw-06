#pragma once

#include "BaseDimension.h"

namespace Homework {

	/// <summary>
	/// Holds an element of the matrix.
	/// </summary>
	/// <typeparam name="T">A type of element. It can be any primitive or object type</typeparam>
	template<typename T, T defaultValue>
	class ElementHolder {
	private:
		using ParentDimension = BaseDimension<ElementHolder>;

		ParentDimension& parentDimension; //a dimension which stores this element
		T value = defaultValue;
		size_t currentIndex;

	public:
		ElementHolder(ParentDimension& parentDimension_, size_t currentIndex_) 
			: parentDimension(parentDimension_), currentIndex(currentIndex_) {
		}

		ElementHolder(const ElementHolder&) = delete;

		ElementHolder(ElementHolder&&) = delete;

		~ElementHolder() = default;

		ElementHolder& operator=(const ElementHolder&) = delete;

		ElementHolder& operator=(ElementHolder&&) = delete;

		ElementHolder& operator=(T newValue) {
			value = newValue;
			if (newValue == defaultValue) {
				parentDimension.erase(currentIndex);
			} else {
				//A new value has been assigned to ElementHolder. Inform the parent dimension about this.
				parentDimension.addElementToMatrix();
			}
			return *this;
		}

		operator T() {
			return value;
		}
	};

};
