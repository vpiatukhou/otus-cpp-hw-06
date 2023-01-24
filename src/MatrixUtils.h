#pragma once

namespace Homework {

	/**
	 * @brief  A utility to create a tuple type with the given number of elements.
	 *
	 * E.g. the following call:
	 *
	 * MatrixElementWithIndicesCreator<double, 3>::Type
	 *
	 * will produce the following tuple:
	 *
	 * std::tuple<size_t, size_t, size_t, double>
	 *
	 * @tparam LastElementType  - a type of the last element in the tuple
	 * @tparam numberOfIndices 	- number of size_t parameters
	 * @tparam IndexTypes		- a list of size_t parameters. A new element is added with an each iteration
	 */
	template<typename LastElementType, size_t numberOfIndices, typename...IndexTypes>
	struct MatrixElementWithIndicesCreator {
		using Type = typename MatrixElementWithIndicesCreator<LastElementType, numberOfIndices - 1, size_t, IndexTypes...>::Type;
	};

	template<typename LastElementType, typename...IndexTypes>
	struct MatrixElementWithIndicesCreator<LastElementType, 0, IndexTypes...> {
		using Type = std::tuple<IndexTypes..., LastElementType>;
	};

};