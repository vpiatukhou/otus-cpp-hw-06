#pragma once

#include "MatrixUtils.h"

#include <cstddef>
#include <tuple>

namespace Homework {

	/**
	 * The struct creates a type of a value which is returned by MatrixForwardIterator.
	 *
	 * The type is "tuple<size_t, size_t, size_t, ..., T>".
	 * 
	 * The number of size_t equals to the number of dimensions.
	 *
	 * @param T		     - a type of an element of the matrix
	 * @param dimensions - a number of dimensions
	 * @param IndexTypes - a list of size_t parameters. A new parameter is added with an each iteration.
	 */
	template<typename T, std::size_t dimensions, typename...IndexTypes>
	struct MatrixIteratorValueTypeCreator {
		using Type = typename MatrixIteratorValueTypeCreator<T, dimensions - 1, std::size_t, IndexTypes...>::Type;
	};

	template<typename T, typename...IndexTypes>
	struct MatrixIteratorValueTypeCreator<T, 0, IndexTypes...> {
		using Type = std::tuple<IndexTypes..., T>;
	};

	/**
	 * Iterates though all elements of all dimensions of the matrix.
	 *
	 * @param T 	       - a type of an element of the matrix
	 * @param defaultValue - a default value
	 * @param dimensions   - a number of dimensions
	 */
	template<typename T, T defaultValue, std::size_t dimensions>
	class MatrixForwardIterator {
	private:
		using IteratorValue = typename MatrixIteratorValueTypeCreator<T, dimensions>::Type;
		using DataIterator = typename DataTypeCreator<T, defaultValue, dimensions>::DataType::iterator;

		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = IteratorValue;
		using pointer = value_type*;
		using reference = value_type&;

		DataIterator dataIterator;
		DataIterator dataEndIterator;

		MatrixForwardIterator<T, defaultValue, dimensions - 1> innerMatrixIterator;

		value_type currentValue;

	public:
		void updateCurrentValue() {
			std::tuple<std::size_t> currentIndex = dataIterator->first;
			currentValue = std::tuple_cat(currentIndex, *innerMatrixIterator);
		}

		void setUpInnerMatrixIterator() {
			innerMatrixIterator = dataIterator->second->begin();
		}

		MatrixForwardIterator() {
		}

		MatrixForwardIterator(const DataIterator& dataBeginIterator_, const DataIterator& dataEndIterator_)
			: dataIterator(dataBeginIterator_), dataEndIterator(dataEndIterator_) {
			if (dataIterator != dataEndIterator) {
				setUpInnerMatrixIterator();
				updateCurrentValue();
			}
		}

		reference operator*() {
			return currentValue;
		}

		MatrixForwardIterator& operator++() {
			if (innerMatrixIterator.hasNext()) {
				++innerMatrixIterator;
				if (innerMatrixIterator.hasNext()) {
					updateCurrentValue();
					return *this;
				}
			}
			++dataIterator;
			if (hasNext()) {
				setUpInnerMatrixIterator();
				updateCurrentValue();
			}
			return *this;
		}

		MatrixForwardIterator operator++(int) {
			MatrixForwardIterator iterator = *this;
			++(*this);
			return iterator;
		}

		bool hasNext() const {
			return innerMatrixIterator.hasNext() || dataIterator != dataEndIterator;
		}

		friend bool operator==(const MatrixForwardIterator& a, const MatrixForwardIterator& b) {
			return a.dataIterator == b.dataIterator && ((a.dataIterator == a.dataEndIterator) || (a.innerMatrixIterator == b.innerMatrixIterator));
		}

		friend bool operator!=(const MatrixForwardIterator& a, const MatrixForwardIterator& b) {
			return !(a == b);
		}
	};

	/**
	 * Iterates though all elements of the matrix.
	 *
	 * @tparam ValueType 		- a type of an element of the matrix
	 * @tparam DataTypeIterator - BaseDimension::DataType::iterator
	 */
	template<typename T, T defaultValue>
	class MatrixForwardIterator<T, defaultValue, 1> {
	private:
		using IteratorValue = typename MatrixIteratorValueTypeCreator<T, 1>::Type;
		using DataIterator = typename DataTypeCreator<T, defaultValue, 1>::DataType::iterator;

		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = IteratorValue;
		using pointer = value_type*;
		using reference = value_type&;

		DataIterator dataIterator;
		DataIterator dataEndIterator;

		value_type currentValue;

	public:
		/**
		 * Fills in two last fields in the given tuple: the value of the element and its last index.
		 *
		 * E.g. if matrix[5][3] = 20, the two last elements of the given tuple will contain 3 and 20.
		 *
		 * @param elementWithIndices - a tuple which contains information about element
		 */
		void updateCurrentValue() {
			//an index of the element
			std::get<0>(currentValue) = dataIterator->first;
			//a value of the element
			std::get<1>(currentValue) = (*(dataIterator->second)).getValue();
		}

		MatrixForwardIterator() {
		}

		MatrixForwardIterator(const DataIterator& dataBeginIterator_, const DataIterator& dataEndIterator_)
			: dataIterator(dataBeginIterator_), dataEndIterator(dataEndIterator_) {
			if (dataIterator != dataEndIterator) {
				updateCurrentValue();
			}
		}

		reference operator*() {
			return currentValue;
		}

		MatrixForwardIterator& operator++() {
			++dataIterator;
			if (hasNext()) {
				updateCurrentValue();
			}
			return *this;
		}

		MatrixForwardIterator operator++(int) {
			MatrixForwardIterator iterator = *this;
			++(*this);
			return iterator;
		}

		bool hasNext() const {
			return dataIterator != dataEndIterator;
		}

		friend bool operator==(const MatrixForwardIterator& a, const MatrixForwardIterator& b) {
			return a.dataIterator == b.dataIterator;
		}

		friend bool operator!=(const MatrixForwardIterator& a, const MatrixForwardIterator& b) {
			return !(a == b);
		}
	};

};
