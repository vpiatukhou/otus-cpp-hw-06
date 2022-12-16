#pragma once

#include "Dimension.h"
#include "MatrixForwardIterator.h"

#include <unordered_map>
#include <memory>
#include <tuple>

namespace Homework {

	/**
	 * @brief The matrix is implemented in the following way.
	 * 
	 * E.g. there is a code:
	 * 
	 * Matrix<int, -1, 5> matrix; //a 5D matrix
	 * 
	 * The following classes will be generated:
	 * 
	 * Matrix<...>
	 * Dimension<..., 4, ...> // "4" is a number of child Dimensions
	 * Dimension<..., 3, ...>
	 * Dimension<..., 2, ...>
	 * Dimension<..., 1, ...>
	 * ElementHolder
	 * 
	 * A Matrix class contains Dimensions which contain other Dimensions and so on.
	 * 
	 * For more information please see the following methods:
	 * 
	 * //get and create an element
	 * BaseDimension::operator[]
	 * BaseDimension::addNewElementToData()
	 * 
	 * //remove an element
	 * BaseDimension::erase()
	 * 
	 * //iterate all elements
	 * Matrix::begin()
	 * Matrix::end()
	 * 
	 * @tparam T 					a type of element of the matrix. Only integral types are supported at the moment.
	 * @tparam defaultValue 		a default value which is returned if a requested element doesn't exist.
	 * @tparam numberOfDimensions 	a number of dimensions. E.g. numberOfDimensions = 5 for 5D matrix.
	 */
	template<typename T, T defaultValue = 0, size_t numberOfDimensions = 2>
	class Matrix : public BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, numberOfDimensions>> {
	public:
		using IteratorValue = typename MatrixElementWithIndicesCreator<T, numberOfDimensions>::Type;
		using DataTypeIterator = typename BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, numberOfDimensions>>::DataType::iterator;
		using ForwardIterator = MatrixForwardIterator<IteratorValue, DataTypeIterator>;

		Matrix() = default;

		void addNewElementToData() override {
			if (this->newElement != nullptr) {
				this->data[this->newElementIndex] = std::move(this->newElement);
				this->newElement = nullptr;
			}
		}

		void erase(size_t index) override {
			this->data.erase(index);
		}

		size_t size() const {
			size_t totalSize = 0;
			for (const auto& indexAndValue : this->data) {
				totalSize += indexAndValue.second->size();
			}
			return totalSize;
		}

		ForwardIterator begin() {
			return ForwardIterator(this->data.begin(), this->data.end());
		}

		ForwardIterator end() {
			return ForwardIterator(this->data.end(), this->data.end());
		}
	};

};
