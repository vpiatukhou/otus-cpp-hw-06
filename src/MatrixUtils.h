#pragma once

#include "MatrixType.h"

#include <unordered_map>
#include <cstddef>
#include <memory>

namespace Homework {

	/**
	 * The struct creates types which are used by Matrix class.
	 */
	template<typename T, T defaultValue, std::size_t dimensions>
	struct DataTypeCreator {
		/**
		 * An element of the matrix. It is a matrix of smaller dimension.
		 */
		using ElementType = Matrix<T, defaultValue, dimensions - 1>;
		/**
	     * A container of elements of the matrix. All elements are matrices of smaller dimension.
	     */
		using DataType = std::unordered_map<std::size_t, std::unique_ptr<ElementType>>;
	};

	/**
	 * The specialization for a 1-dimension matrix.
	 */
	template<typename T, T defaultValue>
	struct DataTypeCreator<T, defaultValue, 1> {

		using ElementType = ElementHolder<T, defaultValue>;
		using DataType = std::unordered_map<std::size_t, std::unique_ptr<ElementType>>;
	};

};