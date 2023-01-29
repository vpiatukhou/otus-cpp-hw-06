#pragma once

#include "MatrixType.h"

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
		BaseMatrix& parent; //an 1-dimension matrix which holds this element
		T value = defaultValue;
		size_t currentIndex; //an index of the current element in the dimension

	public:
		ElementHolder(BaseMatrix& parent_, size_t currentIndex_) : parent(parent_), currentIndex(currentIndex_) {
		}

		ElementHolder(const ElementHolder&) = delete;
		ElementHolder(ElementHolder&&) = delete;
		~ElementHolder() = default;

		ElementHolder& operator=(const ElementHolder&) = delete;
		ElementHolder& operator=(ElementHolder&&) = delete;

		ElementHolder& operator=(T newValue) {
			value = newValue;
			if (newValue == defaultValue) {
				parent.erase(currentIndex);
			} else {
				//A new value has been assigned to ElementHolder. Inform the parent dimension about this.
				parent.addNewElementToData();
			}
			return *this;
		}

		operator T() {
			return value;
		}

		const T& getValue() const {
			return value;
		}

		std::size_t getCurrentIndex() const {
			return currentIndex;
		}
	};

};
