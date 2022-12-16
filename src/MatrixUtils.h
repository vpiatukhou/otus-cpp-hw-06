#pragma once

/// <summary>
/// A utility to create a type which store indicies and element value.
/// 
/// The result looks like:
/// 
/// std::tuple<size_t, size_t, size_t ..., EndElementType>
/// 
/// Here size_t is repeated "numberOfIndices" times.
/// 
/// </summary>
/// <typeparam name="...IndexTypes">do not use this parameter. It is used internally to generate a tuple</typeparam>
/// <typeparam name="numberOfIndices">number of dimensions in the matrix</typeparam>
/// <typeparam name="LastElementType">a type of the last element in the tuple</typeparam>
template<typename LastElementType, size_t numberOfIndices, typename...IndexTypes>
struct MatrixElementWithIndiciesCreator {
	using Type = typename MatrixElementWithIndiciesCreator<LastElementType, numberOfIndices - 1, size_t, IndexTypes...>::Type;
};

template<typename LastElementType, typename...IndexTypes>
struct MatrixElementWithIndiciesCreator<LastElementType, 0, IndexTypes...> {
	using Type = std::tuple<IndexTypes..., LastElementType>;
};
