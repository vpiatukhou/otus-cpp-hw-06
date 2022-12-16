#pragma once

#include <unordered_map>
#include <memory>

namespace Homework {

	/// <summary>
	/// A base class which represents a signle dimension of the matrix.
	/// </summary>
	/// <typeparam name="T">A type of element of the dimension. It can be another Dimension or ElementHolder</typeparam>
	template<class T>
	class BaseDimension {
	protected:
		using ElementType = T; //we create alias for T in order to re-use it in subclasses
		using DataType = std::unordered_map<size_t, std::unique_ptr<ElementType>>;

		DataType data;
		std::unique_ptr<ElementType> newElement = nullptr;
		size_t newElementIndex = 0;

	public:
		virtual ~BaseDimension() {
		};

		virtual void addElementToMatrix() = 0;

		virtual void erase(size_t index) = 0;

		ElementType& operator[](size_t index) {
			if (data.find(index) == data.end()) {
				newElementIndex = index;
				newElement = std::make_unique<ElementType>(*this, index);
				return *newElement;
			}
			return *(data[index]);
		};
	};

};
