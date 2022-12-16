#pragma once

#include "BaseDimension.h"
#include "ElementHolder.h"
#include "DimensionIterator.h"
#include "MatrixUtils.h"

namespace Homework {

	/**
	 * @brief A dimension of the matrix.
	 * 
	 * A single specialization is generated for an each dimension in the matrix.
	 * 
	 * E.g. for 5D matrix, the following classes will be generated:
	 * 
	 * Dimension<T, defaultValue, 4, ...> 
	 * Dimension<T, defaultValue, 3, ...>
	 * Dimension<T, defaultValue, 2, ...>
	 * Dimension<T, defaultValue, 1, ...>
	 * 
	 * An each of the classes (except the last one) contains other dimensions.
	 * 
	 * The last one contains elements of the matrix. Please see the specialization Dimension<T, defaultValue, 1, ...> below in this file.
	 * 
	 * @tparam T 						- a type of elements of the matrix
	 * @tparam defaultValue 			- a default value of elements of the matrix
	 * @tparam numberOfDimensions 		- a number of dimensions within this dimension
	 * @tparam matrixNumberOfDimensions - a total number of dimensions of the matrix
	 */
	template<typename T, T defaultValue, size_t numberOfDimensions, size_t matrixNumberOfDimensions>
	class Dimension : public BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, matrixNumberOfDimensions>> {
	private:
		using ParentDimension = BaseDimension<Dimension<T, defaultValue, numberOfDimensions, matrixNumberOfDimensions>>;
		using ElementWithIndices = typename MatrixElementWithIndicesCreator<T, matrixNumberOfDimensions>::Type;

		//a dimension which holds this dimenstion
		ParentDimension& parentDimension;
		//an index of this dimension in the parent dimension
		size_t currentIndex = 0;

	public:
		Dimension(ParentDimension& parentDimension_, size_t currentIndex_) : parentDimension(parentDimension_), currentIndex(currentIndex_) {
		}

		Dimension() = default;

		void addNewElementToData() override {
			if (this->newElement != nullptr) {
				this->data[this->newElementIndex] = std::move(this->newElement);
				this->newElement = nullptr;
				parentDimension.addNewElementToData();
			}
		}

		void erase(size_t index) override {
			this->data.erase(index);
			if (this->data.empty()) {
				parentDimension.erase(this->currentIndex);
			}
		}

		size_t size() const {
			size_t totalSize = 0;
			for (const auto& indexAndValue : this->data) {
				totalSize += indexAndValue.second->size();
			}
			return totalSize;
		}

		std::unique_ptr<DimensionIterator<ElementWithIndices>> begin() {
			using DataTypeIterator = typename BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, matrixNumberOfDimensions>>::DataType::iterator;

			return std::make_unique<DimensionIteratorImpl<T, DataTypeIterator, ElementWithIndices, numberOfDimensions, matrixNumberOfDimensions> >(
				this->data.begin(), this->data.end());
		}
	};

	/**
	 * @brief This specialization of Dimension class represents the most deepest dimension of the matrix. 
	 * 
	 * E.g. there is a matrix:
	 * 
	 * matrix[5][2][3] = 100
	 * 
	 * This class represents a dimension with index "2" which contains the element with value "100".
	 * 
	 * @tparam T 						- a type of elements of the matrix
	 * @tparam defaultValue 			- a default value of elements
	 * @tparam matrixNumberOfDimensions - a total number of dimensions of the matrix
	 */
	template<typename T, T defaultValue, size_t matrixNumberOfDimensions>
	struct Dimension<T, defaultValue, 1, matrixNumberOfDimensions> : public BaseDimension<ElementHolder<T, defaultValue>> {
	private:
		using ParentDimension = BaseDimension<Dimension<T, defaultValue, 1, matrixNumberOfDimensions>>;
		using ElementWithIndices = typename MatrixElementWithIndicesCreator<T, matrixNumberOfDimensions>::Type;

		ParentDimension& parentDimension = nullptr;
		size_t currentIndex = 0;

	public:
		Dimension(ParentDimension& parentDimension_, size_t currentIndex_) 
			: parentDimension(parentDimension_), currentIndex(currentIndex_) {
		}

		Dimension() = default;

		void addNewElementToData() override {
			if (this->newElement != nullptr) {
				this->data[this->newElementIndex] = std::move(this->newElement);
				this->newElement = nullptr;
				parentDimension.addNewElementToData();
			}
		}

		void erase(size_t index) override {
			this->data.erase(index);
			if (this->data.empty()) {
				parentDimension.erase(currentIndex);
			}
		}

		size_t size() const {
			return this->data.size();
		}

		std::unique_ptr<DimensionIterator<ElementWithIndices>> begin() {
			using DataTypeIterator = typename BaseDimension<ElementHolder<T, defaultValue>>::DataType::iterator;

			return std::make_unique<DimensionIteratorImpl<T, DataTypeIterator, ElementWithIndices, 1, matrixNumberOfDimensions>>(
				this->data.begin(), this->data.end());
		}
	};

};
