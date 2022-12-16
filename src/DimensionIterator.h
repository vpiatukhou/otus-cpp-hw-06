#pragma once

#include <memory>

namespace Homework {

	/// <summary>
	/// Provides an interface to iterate all elements (including children) within Dimension.
	/// </summary>
	/// <typeparam name="ElementWithIndicies">contains indicies of element and its value</typeparam>
	template<typename ElementWithIndicies>
	class IDimensionIterator {
	public:
		virtual ~IDimensionIterator() {
		}

		virtual bool next() = 0;

		virtual void populateElementWithIndicies(ElementWithIndicies& elementWithIndicies) = 0;
	};

	/// <summary>
	/// Iterates through all elements of dimension and their child dimensions.
	/// </summary>
	template<typename T, class DataTypeIterator, class ElementWithIndicies, size_t numberOfDimensions, size_t matrixNumberOfDimensions>
	class DimensionIterator : public IDimensionIterator<ElementWithIndicies> {
	private:
		DataTypeIterator dataBeginIterator;
		DataTypeIterator dataEndIterator;
		std::unique_ptr<IDimensionIterator<ElementWithIndicies>> childDimensionIterator;

	public:
		DimensionIterator(const DataTypeIterator& dataBeginIterator_, const DataTypeIterator& dataEndIterator_)
			: dataBeginIterator(dataBeginIterator_), dataEndIterator(dataEndIterator_) {
			childDimensionIterator = dataBeginIterator->second->begin();
		}

		bool next() override {
			if (dataBeginIterator == dataEndIterator) {
				return false;
			}

			bool hasNext = childDimensionIterator->next();
			if (!hasNext) {
				++dataBeginIterator;
				if (dataBeginIterator != dataEndIterator) {
					childDimensionIterator = dataBeginIterator->second->begin();
					hasNext = true;
				}
			}
			return hasNext;
		}

		void populateElementWithIndicies(ElementWithIndicies& elementWithIndicies) override {
			std::get<matrixNumberOfDimensions - numberOfDimensions>(elementWithIndicies) = dataBeginIterator->first;
			childDimensionIterator->populateElementWithIndicies(elementWithIndicies);
		}
	};

	/// <summary>
	/// A specialization for the most deepest dimension of the matrix (the dimension which doesn't contain children).
	/// </summary>
	template<typename T, class DataTypeIterator, class ElementWithIndicies, size_t matrixNumberOfDimensions>
	class DimensionIterator<T, DataTypeIterator, ElementWithIndicies, 1, matrixNumberOfDimensions> : public IDimensionIterator<ElementWithIndicies> {
	private:
		DataTypeIterator dataBeginIterator;
		DataTypeIterator dataEndIterator;

	public:
		DimensionIterator(const DataTypeIterator& dataBeginIterator_, const DataTypeIterator& dataEndIterator_)
			: dataBeginIterator(dataBeginIterator_), dataEndIterator(dataEndIterator_) {
		}

		bool next() override {
			if (dataBeginIterator != dataEndIterator) {
				++dataBeginIterator;
				return dataBeginIterator != dataEndIterator;
			}
			return false;
		}

		/// <summary>
		/// Fills in two last fields in the given tuple: the value of the element and its last index.
		/// 
		/// E.g. if matrix[5][3] = 20, the two last elements of the given tuple will contain 3 and 20.
		/// </summary>
		/// <param name="result"></param>
		void populateElementWithIndicies(ElementWithIndicies& elementWithIndicies) override {
			//an index of the element
			std::get<matrixNumberOfDimensions - 1>(elementWithIndicies) = dataBeginIterator->first;
			//a value of the element
			std::get<matrixNumberOfDimensions>(elementWithIndicies) = static_cast<T>(*dataBeginIterator->second);
		}
	};

};
