#pragma once

#include <memory>

namespace Homework {

	/**
	 * @brief Provides an interface to iterate all elements (including children) within Dimension.
	 * 
	 * @tparam ElementWithIndices - contains all indices of an element and its value
	 */
	template<typename ElementWithIndices>
	class DimensionIterator {
	public:
		virtual ~DimensionIterator() {
		}

		/**
		 * @brief move the iterator forward.
		 * 
		 * @return true  - if the iterator points to an element
		 * @return false - if the iterator doesn't point to an element
		 */
		virtual bool next() = 0;

		/**
		 * @brief fills in the given tuple with values.
		 * 
		 * @param elementWithIndices - the tuple which contains indices of an element and its value
		 * @see MatrixElementWithIndicesCreator in MatrixUtils.h
		 */
		virtual void populateElementWithIndices(ElementWithIndices& elementWithIndices) = 0;
	};

	/**
	 * @brief Iterates through all elements of dimension and their child dimensions.
	 * 
	 * @tparam T 						- a type of element in the matrix
	 * @tparam DataTypeIterator 		- BaseDimension::DataType::iterator
	 * @tparam ElementWithIndices 		- the tuple which contains indices of an element and its value
	 * @tparam numberOfDimensions 		- a number of child dimensions in the current dimension
	 * @tparam matrixNumberOfDimensions - a total number of dimensions of the matrix
	 */
	template<typename T, class DataTypeIterator, class ElementWithIndices, size_t numberOfDimensions, size_t matrixNumberOfDimensions>
	class DimensionIteratorImpl : public DimensionIterator<ElementWithIndices> {
	private:
		DataTypeIterator dataBeginIterator;
		DataTypeIterator dataEndIterator;
		std::unique_ptr<DimensionIterator<ElementWithIndices>> childDimensionIterator;

	public:
		DimensionIteratorImpl(const DataTypeIterator& dataBeginIterator_, const DataTypeIterator& dataEndIterator_)
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

		void populateElementWithIndices(ElementWithIndices& elementWithIndices) override {
			std::get<matrixNumberOfDimensions - numberOfDimensions>(elementWithIndices) = dataBeginIterator->first;
			childDimensionIterator->populateElementWithIndices(elementWithIndices);
		}
	};

	/**
	 * @brief A specialization for the most deepest dimension of the matrix (the dimension which doesn't contain children).
	 *  
	 * @tparam T 						- a type of element in the matrix
	 * @tparam DataTypeIterator 		- Dimension::DataType::iterator
	 * @tparam ElementWithIndices 		- the tuple which contains indices of an element and its value
	 * @tparam matrixNumberOfDimensions - a total number of dimensions of the matrix
	 */
	template<typename T, class DataTypeIterator, class ElementWithIndices, size_t matrixNumberOfDimensions>
	class DimensionIteratorImpl<T, DataTypeIterator, ElementWithIndices, 1, matrixNumberOfDimensions> 
		: public DimensionIterator<ElementWithIndices> {
	private:
		DataTypeIterator dataBeginIterator;
		DataTypeIterator dataEndIterator;

	public:
		DimensionIteratorImpl(const DataTypeIterator& dataBeginIterator_, const DataTypeIterator& dataEndIterator_)
			: dataBeginIterator(dataBeginIterator_), dataEndIterator(dataEndIterator_) {
		}

		bool next() override {
			if (dataBeginIterator != dataEndIterator) {
				++dataBeginIterator;
				return dataBeginIterator != dataEndIterator;
			}
			return false;
		}

		/**
		 * @brief Fills in two last fields in the given tuple: the value of the element and its last index.
		 * 
		 * E.g. if matrix[5][3] = 20, the two last elements of the given tuple will contain 3 and 20.
		 * 
		 * @param elementWithIndices - a tuple which contains information about element
		 */
		void populateElementWithIndices(ElementWithIndices& elementWithIndices) override {
			//an index of the element
			std::get<matrixNumberOfDimensions - 1>(elementWithIndices) = dataBeginIterator->first;
			//a value of the element
			std::get<matrixNumberOfDimensions>(elementWithIndices) = static_cast<T>(*dataBeginIterator->second);
		}
	};

};
