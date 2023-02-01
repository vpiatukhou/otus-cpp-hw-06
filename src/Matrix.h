#pragma once

#include "ElementHolder.h"
#include "MatrixForwardIterator.h"
#include "MatrixType.h"
#include "MatrixUtils.h"

#include <stdexcept>

namespace Homework {

	/**
	 * An N-dimensional infinite sparse matrix.
	 * 
	 * Usage:
	 * 
	 * Matrix<int, -1, 3> m;
	 * m[3][5][2] = 10;
	 * int e1 = m[3][5][2];	   // e1 = 10
	 * int e2 = m[3][5][100];  // e2 = -1
	 * 
	 * ATTENTION:
	 *
	 * Never obtain a reference to the element of the matrix:
	 *
	 * auto& e1 = m[1][2];
	 * auto& e2 = m[3][10];
	 * cout << e1 << endl; // WARNING: undefined behavior
	 * cout << e2 << endl; // e2 = -1
	 *
	 * @param T				- a type of an element of the matrix. At the moment only integral types are supported.
	 * @param defaultValue	- this value will be returned if there is no any element with the given indices
	 * @param dimensions	- a number of dimensions
	 */
	template<typename T, T defaultValue, std::size_t dimensions = 2>
	class Matrix : public BaseMatrix {
	private:
		//an inner matrix with one less number of dimensions.
		using ElementType = typename DataTypeCreator<T, defaultValue, dimensions>::ElementType;
		//a container which holds the inner matrices
		using DataType = typename DataTypeCreator<T, defaultValue, dimensions>::DataType;
		using Iterator = MatrixForwardIterator<T, defaultValue, dimensions>;

		//a raw pointer is used to simplify the code. The parent cannot be deleted before the child, the pointer will never have the undefined value.
		BaseMatrix* parent = nullptr;
		std::size_t currentIndex = 0;

		DataType data;

		std::unique_ptr<ElementType> newElement = nullptr;

	public:
		Matrix() = default;

		Matrix(BaseMatrix* parent_, size_t currentIndex_) : parent(parent_), currentIndex(currentIndex_) {
		}

		void addNewElementToData() override {
			if (newElement != nullptr) {
				auto index = newElement->getCurrentIndex();
				data[index] = std::move(newElement);
				newElement = nullptr;
				if (parent != nullptr) {
					parent->addNewElementToData();
				}
			}
		}

		void erase(std::size_t index) override {
			data.erase(index);
			if (data.empty()) {
				parent->erase(currentIndex);
			}
		}

		/**
		 * Returns an element (an inner matrix) with the given index. 
		 * If there is no element with such index, a new one will be created.
		 * 
		 * @return an inner matrix with the given index
		 */
		ElementType& operator[](std::size_t index) {
			if (data.find(index) == data.end()) {
				//If an element doesn't exist, a new one is created and saved it in the variable.
				//The element will be added to the matrix later using addNewElementToData().
				newElement = std::make_unique<ElementType>(this, index);
				return *newElement;
			}
			return *(data[index]);
		};

		size_t size() const {
			size_t totalSize = 0;
			for (const auto& indexAndValue : this->data) {
				totalSize += indexAndValue.second->size();
			}
			return totalSize;
		}

		std::size_t getCurrentIndex() const {
			return currentIndex;
		}

		Iterator begin() {
			return Iterator(data.begin(), data.end());
		}

		Iterator end() {
			return Iterator(data.end(), data.end());
		}
	};

	template<typename T, T defaultValue>
	class Matrix<T, defaultValue, 1> : public BaseMatrix {
		using ElementType = typename DataTypeCreator<T, defaultValue, 1>::ElementType;
		using DataType = typename DataTypeCreator<T, defaultValue, 1>::DataType;
		using Iterator = MatrixForwardIterator<T, defaultValue, 1>;

		//a raw pointer is used to simplify the code. The parent cannot be deleted before the child, the pointer will never have the undefined value.
		BaseMatrix* parent = nullptr;
		std::size_t currentIndex = 0;

		DataType data;
		std::unique_ptr<ElementType> newElement = nullptr;

	public:
		Matrix() = default;

		Matrix(BaseMatrix* parent_, std::size_t currentIndex_) : parent(parent_), currentIndex(currentIndex_) {
		}

		void addNewElementToData() override {
			if (newElement != nullptr) {
				auto index = newElement->getCurrentIndex();
				data[index] = std::move(newElement);
				newElement = nullptr;
				parent->addNewElementToData();
			}
		}

		void erase(std::size_t index) override {
			data.erase(index);
			if (data.empty()) {
				parent->erase(currentIndex);
			}
		}

		ElementType& operator[](size_t index) {
			if (data.find(index) == data.end()) {
				//If an element doesn't exist, we create a new one and save it in the variable.
				//The element will be added to the matrix later using addNewElementToData().
				newElement = std::make_unique<ElementType>(*this, index);
				return *newElement;
			}
			return *(data[index]);
		};

		size_t size() const {
			return data.size();
		}

		std::size_t getCurrentIndex() const {
			return currentIndex;
		}

		Iterator begin() {
			return Iterator(data.begin(), data.end());
		}

		Iterator end() {
			return Iterator(data.end(), data.end());
		}
	};
};