#pragma once

#include "BaseDimension.h"

namespace Homework {

	/**
	 * @brief Holds an element of the matrix.
	 * 
	 * @tparam T            - a type of the element
	 * @tparam defaultValue - a default value of the element
	 */
	template<typename T, T defaultValue>
	class ElementHolder {
	private:
		using ParentDimension = BaseDimension<ElementHolder>;

		ParentDimension& parentDimension; //a dimension which holds this element
		T value = defaultValue;
		size_t currentIndex; //an index of the current element in the dimension

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
				parentDimension.addNewElementToData();
			}
			return *this;
		}

		operator T() {
			return value;
		}
	};

};
