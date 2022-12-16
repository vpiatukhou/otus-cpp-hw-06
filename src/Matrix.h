#pragma once

#include "Dimension.h"

#include <unordered_map>
#include <memory>
#include <tuple>

namespace Homework {

	template<typename T, T defaultValue, size_t numberOfDimensions = 2>
	class Matrix : public BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, numberOfDimensions>> {
	public:
		class ForwardIterator {
		private:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = typename MatrixElementWithIndiciesCreator<T, numberOfDimensions>::Type;
			using pointer = value_type*;
			using reference = value_type&;

			using DataTypeIterator = typename BaseDimension<Dimension<T, defaultValue, numberOfDimensions - 1, numberOfDimensions>>::DataType::iterator;

			DataTypeIterator dataBeginIterator;
			DataTypeIterator dataEndIterator;
			std::unique_ptr<IDimensionIterator<value_type>> dimensionIterator = nullptr;
			value_type currentValue;

		public:
			void populateElementWithIndicies() {
				std::get<0>(currentValue) = dataBeginIterator->first;
				dimensionIterator->populateElementWithIndicies(currentValue);
			}

			ForwardIterator(DataTypeIterator dataBeginIterator_, DataTypeIterator dataEndIterator_)
				: dataBeginIterator(dataBeginIterator_), dataEndIterator(dataEndIterator_) {
				if (dataBeginIterator != dataEndIterator) {
					dimensionIterator = dataBeginIterator->second->begin();
					populateElementWithIndicies();
				}
			}

			reference operator*() {
				return currentValue;
			}

			ForwardIterator& operator++() {
				if (dataBeginIterator == dataEndIterator) {
					return *this;
				}
				if (dimensionIterator->next()) {
					populateElementWithIndicies();
				} else {
					dimensionIterator = nullptr;
					++dataBeginIterator;
					if (dataBeginIterator != dataEndIterator) {
						dimensionIterator = dataBeginIterator->second->begin();
						populateElementWithIndicies();
					}
				}
				return *this;
			}

			ForwardIterator operator++(int) {
				ForwardIterator iterator = *this;
				++(*this);
				return iterator;
			}

			friend bool operator==(const ForwardIterator& a, const ForwardIterator& b) {
				return a.dataBeginIterator == b.dataBeginIterator && a.dimensionIterator == b.dimensionIterator;
			}

			friend bool operator!=(const ForwardIterator& a, const ForwardIterator& b) {
				return !(a == b);
			}
		};

		Matrix() = default;

		~Matrix() = default;

		Matrix(const Matrix&) = delete;

		Matrix(Matrix&&) = delete;

		Matrix& operator=(const Matrix&) = delete;

		Matrix& operator=(Matrix&&) = delete;

		void addElementToMatrix() override {
			if (this->newElement != nullptr) {
				this->data[this->newElementIndex] = std::move(this->newElement);
				this->newElement = nullptr;
			}
		}

		void erase(size_t index) override {
			this->data.erase(index);
		}

		size_t size() const {
			size_t totalSize = 0;
			for (const auto& indexAndValue : this->data) {
				totalSize += indexAndValue.second->size();
			}
			return totalSize;
		}

		ForwardIterator begin() {
			return ForwardIterator(this->data.begin(), this->data.end());
		}

		ForwardIterator end() {
			return ForwardIterator(this->data.end(), this->data.end());
		}
	};

};
