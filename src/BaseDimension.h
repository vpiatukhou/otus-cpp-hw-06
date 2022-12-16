#pragma once

#include <unordered_map>
#include <memory>

namespace Homework {

	/**
	 * @brief A base class which represents a signle dimension of the matrix.
	 * 
	 * @tparam ElementType - a type of element in the dimension. It can be another Dimension or ElementHolder.
	 */
	template<class ElementType>
	class BaseDimension {
	protected:
		using DataType = std::unordered_map<size_t, std::unique_ptr<ElementType>>;

		DataType data;
		std::unique_ptr<ElementType> newElement = nullptr;
		size_t newElementIndex = 0;

	public:
		virtual ~BaseDimension() {
		};

		BaseDimension() = default;

		BaseDimension(const BaseDimension&) = delete;

		BaseDimension(BaseDimension&&) = delete;

		BaseDimension& operator=(const BaseDimension&) = delete;

		BaseDimension& operator=(BaseDimension&&) = delete;

		/**
		 * @brief Adds newElement to the data. 
		 * 
		 * The user of the Matrix class must not call this method directly. It is automatically called when a new value is assigned to ElementHolder.
		 * 
		 * Please see the description of creation process below:
		 * 
		 * E.g. there is an empty matrix. Let's add an element with the value "3".
		 * 
		 * matrix[3][2] = 10;
		 * 
		 * The sequence of calls is:
		 * 
		 * BaseDimension::operator[](3) // returns a new Dimension object
		 * BaseDimension::operator[](2) // returns a new ElementHolder object
		 * ElementHolder::operator=(10)	// store the value "10" in ElementHolder and call addNewElementToData() of the parent Dimensions.
		 */
		virtual void addNewElementToData() = 0;

		/**
		 * @brief Removes an element with the given index.
		 * 
		 * The user of the Matrix class must not call this method directly. It is automatically called when a default value is assigned to ElementHolder.
		 * 
		 * E.g. there is a matrix with two elements:
		 * 
		 * matrix[3][2][5] = 10;
		 * matrix[3][3][6] = 20;
		 * 
		 * Assign a default element:
		 * 
		 * matrix[3][2][5] = -1;
		 * 
		 * The sequence of calls is:
		 * 
		 * Dimension::erase(5)
		 * Dimension::erase(2)
		 * //erase(3) won't be called because there is one more element in this Dimension.
		 * 
		 * @param index 
		 */
		virtual void erase(size_t index) = 0;

		/**
		 * @brief Returns an existing element or creates a new one.
		 * 
		 * @param index 		- an index of the element within the dimension
		 * @return ElementType& - a reference to an existing element or a new one
		 */
		ElementType& operator[](size_t index) {
			if (data.find(index) == data.end()) {
				newElementIndex = index;
				//If an element doesn't exist, we create a new one and save it in the variable.
				//The element will be added to the matrix later using addNewElementToData().
				newElement = std::make_unique<ElementType>(*this, index);
				return *newElement;
			}
			return *(data[index]);
		};
	};

};
