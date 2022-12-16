#pragma once

#include "BaseDimension.h"
#include "ElementHolder.h"
#include "DimensionIterator.h"
#include "MatrixUtils.h"

namespace Homework {

	/// <summary>
	/// A dimension of the matrix. It contains other dimensions.
	/// 
	/// If a matrix contains more than two dimensions, a new dimension class will be generated for an each dimension.
	/// 
	/// For a dimension which contains elements please see the specialization of this class below.
	/// </summary>
	/// <typeparam name="T">a type of elements of the matrix</typeparam>
	/// <typeparam name="defaultValue">a default value of elements</typeparam>
	/// <typeparam name="numberOfDimensions">a number of dimensions within this dimension</typeparam>
	/// <typeparam name="matrixNumberOfDimensions">a total number of dimensions of the matrix</typeparam>
	template<typename T, T defaultValue, size_t numberOfDimensions, size_t matrixNumberOfDimensions>
	class Dimension : public BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, matrixNumberOfDimensions>> {
	private:
		using ParentDimension = BaseDimension<Dimension<T, defaultValue, numberOfDimensions, matrixNumberOfDimensions>>;
		using ElementWithIndicies = typename MatrixElementWithIndiciesCreator<T, matrixNumberOfDimensions>::Type;

		ParentDimension& parentDimension;
		size_t currentIndex = 0;

	public:
		Dimension(ParentDimension& parentDimension_, size_t currentIndex_) : parentDimension(parentDimension_), currentIndex(currentIndex_) {
		}

		Dimension() = default;

		~Dimension() = default;

		Dimension(const Dimension&) = delete;

		Dimension(Dimension&&) = delete;

		Dimension& operator=(const Dimension&) = delete;

		Dimension& operator=(Dimension&&) = delete;

		void addElementToMatrix() override {
			if (this->newElement != nullptr) {
				this->data[this->newElementIndex] = std::move(this->newElement);
				this->newElement = nullptr;
				parentDimension.addElementToMatrix();
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

		using DataTypeIterator = typename BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, matrixNumberOfDimensions>>::DataType::iterator;

		std::unique_ptr<IDimensionIterator<ElementWithIndicies>> begin() {
			return std::make_unique<DimensionIterator<T, DataTypeIterator, ElementWithIndicies, numberOfDimensions, matrixNumberOfDimensions> >(
				this->data.begin(), this->data.end());
		}
	};

	/// <summary>
	/// This specialization of Dimension class represents the most deepest dimension of the matrix. 
	/// 
	/// E.g. there is a matrix:
	/// 
	/// matrix[5][2][3] = 100
	/// 
	/// This class represents a dimension with index "2" which contains the element with value "100".
	/// </summary>
	/// <typeparam name="T">a type of elements of the matrix</typeparam>
	/// <typeparam name="defaultValue">a default value of elements</typeparam>
	/// <typeparam name="matrixNumberOfDimensions">a total number of dimensions of the matrix</typeparam>
	template<typename T, T defaultValue, size_t matrixNumberOfDimensions>
	struct Dimension<T, defaultValue, 1, matrixNumberOfDimensions> : public BaseDimension<ElementHolder<T, defaultValue>> {
	private:
		using ParentDimension = BaseDimension<Dimension<T, defaultValue, 1, matrixNumberOfDimensions>>;
		using ElementWithIndicies = typename MatrixElementWithIndiciesCreator<T, matrixNumberOfDimensions>::Type;

		ParentDimension& parentDimension = nullptr;
		size_t currentIndex = 0;

	public:
		Dimension(ParentDimension& parentDimension_, size_t currentIndex_) 
			: parentDimension(parentDimension_), currentIndex(currentIndex_) {
		}

		Dimension() = default;

		~Dimension() = default;

		Dimension(const Dimension&) = delete;

		Dimension(Dimension&&) = delete;

		Dimension& operator=(const Dimension&) = delete;

		Dimension& operator=(Dimension&&) = delete;

		void addElementToMatrix() override {
			if (this->newElement != nullptr) {
				this->data[this->newElementIndex] = std::move(this->newElement);
				this->newElement = nullptr;
				parentDimension.addElementToMatrix();
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

		using DataTypeIterator = typename BaseDimension<ElementHolder<T, defaultValue>>::DataType::iterator;

		std::unique_ptr<IDimensionIterator<ElementWithIndicies>> begin() {
			return std::make_unique<DimensionIterator<T, DataTypeIterator, ElementWithIndicies, 1, matrixNumberOfDimensions>>(
				this->data.begin(), this->data.end());
		}
	};

};
