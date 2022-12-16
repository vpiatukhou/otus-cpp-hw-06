#pragma once

#include "DimensionIterator.h"

#include <iterator>
#include <memory>

namespace Homework {

	/**
	 * @brief Iterates though all elements of the matrix.
	 * 
	 * @tparam ValueType 		- a type of an element of the matrix
	 * @tparam DataTypeIterator - BaseDimension::DataType::iterator
	 */
    template<class ValueType, class DataTypeIterator>
    class MatrixForwardIterator {
	private:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = ValueType;
		using pointer = value_type*;
		using reference = value_type&;

		DataTypeIterator dataBeginIterator;
		DataTypeIterator dataEndIterator;
		std::unique_ptr<DimensionIterator<value_type>> dimensionIterator = nullptr;
        value_type currentValue;

	public:
		void populateElementWithIndices() {
			std::get<0>(currentValue) = dataBeginIterator->first;
			dimensionIterator->populateElementWithIndices(currentValue);
		}

		MatrixForwardIterator(DataTypeIterator dataBeginIterator_, DataTypeIterator dataEndIterator_)
			: dataBeginIterator(dataBeginIterator_), dataEndIterator(dataEndIterator_) {
	    	if (dataBeginIterator != dataEndIterator) {
				dimensionIterator = dataBeginIterator->second->begin();
				populateElementWithIndices();
			}
		}

		reference operator*() {
			return currentValue;
		}

		MatrixForwardIterator& operator++() {
			if (dataBeginIterator == dataEndIterator) {
				return *this;
			}
			if (dimensionIterator->next()) {
				populateElementWithIndices();
			} else {
				dimensionIterator = nullptr;
				++dataBeginIterator;
				if (dataBeginIterator != dataEndIterator) {
					dimensionIterator = dataBeginIterator->second->begin();
					populateElementWithIndices();
				}
			}
			return *this;
		}

		MatrixForwardIterator operator++(int) {
			MatrixForwardIterator iterator = *this;
			++(*this);
            return iterator;
		}

		friend bool operator==(const MatrixForwardIterator& a, const MatrixForwardIterator& b) {
			return a.dataBeginIterator == b.dataBeginIterator && a.dimensionIterator == b.dimensionIterator;
		}

    	friend bool operator!=(const MatrixForwardIterator& a, const MatrixForwardIterator& b) {
			return !(a == b);
		}
    };

};
