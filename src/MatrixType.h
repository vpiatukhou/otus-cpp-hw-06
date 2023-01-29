/**
 * This header contains declarations which are needed in different files.
 */
#pragma once

#include <cstddef>

namespace Homework {

	/**
	 * Declaration of a matrix class.
	 */
    template<typename T, T defaultValue, std::size_t dimensions>
    class Matrix;

	/**
	 * A base class of Matrix. The purpose of this class is to simplify usage of pointers. E.g.:
	 * 
	 * BaseMatrix* parent;
	 * 
	 * instead of
	 * 
	 * Matrix<T, -1, dimensions + 1>* parent;
	 */
	class BaseMatrix {
	public:
		virtual ~BaseMatrix() {
		}

		BaseMatrix() = default;

		BaseMatrix(const BaseMatrix&) = delete;
		BaseMatrix(BaseMatrix&&) = delete;

		BaseMatrix& operator=(const BaseMatrix&) = delete;
		BaseMatrix& operator=(BaseMatrix&&) = delete;

		/**
		 * Adds a new element (which is temporary sorted in this matrix instance) to the matrix.
		 */
		virtual void addNewElementToData() = 0;

		/**
		 * Deletes element with the given index.
		 */
		virtual void erase(std::size_t index) = 0;
	};
};