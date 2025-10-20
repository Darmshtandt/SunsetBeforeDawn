#pragma once

#include <StdH.h>

namespace Algorithms {
	__inline std::vector<Nt::IntRect>::const_iterator FindBestFit(const std::vector<Nt::IntRect>& freeAreas, const Nt::IntRect& rect) noexcept {
		std::vector<Nt::IntRect>::const_iterator bestFreeRect = freeAreas.end();
		Int minArea = INT_MAX;

		for (std::vector<Nt::IntRect>::const_iterator iterator = freeAreas.cbegin(); iterator != freeAreas.cend(); ++iterator) {
			if (iterator->Right < rect.Right || iterator->Bottom < rect.Bottom)
				continue;

			const Int area = (iterator->Right - rect.Right) * (iterator->Bottom - rect.Bottom);
			if (minArea > area) {
				bestFreeRect = iterator;
				minArea = area;
			}
		}

		return bestFreeRect;
	}

	__inline void PlaceRect(std::vector<Nt::IntRect>& freeAreas, std::vector<Nt::IntRect>::const_iterator& bestFreeRectIterator, Nt::IntRect& rect) {
		if (bestFreeRectIterator == freeAreas.cend()) {
			Raise("Incorrect best fit iterator");
			return;
		}

		rect.LeftTop = bestFreeRectIterator->LeftTop;

		const Nt::IntRect firstPart = {
			bestFreeRectIterator->Left + rect.Right,
			bestFreeRectIterator->Top,
			bestFreeRectIterator->Right - rect.Right,
			bestFreeRectIterator->Bottom,
		};
		const Nt::IntRect secondPart = {
			bestFreeRectIterator->Left,
			bestFreeRectIterator->Top + rect.Bottom,
			bestFreeRectIterator->Right,
			bestFreeRectIterator->Bottom - rect.Bottom,
		};

		freeAreas.erase(bestFreeRectIterator);

		freeAreas.push_back(firstPart);
		freeAreas.push_back(secondPart);
	}

	__inline void ExpandAtlas(std::vector<Nt::IntRect>& freeAreas, Nt::IntRect& rect) {

	}

	__inline void MaxRects(std::list<std::reference_wrapper<Nt::IntRect>>& textureAreas, Nt::Int2D& initialSize) {
		auto predicate = [](const std::reference_wrapper<Nt::IntRect>& left, const std::reference_wrapper<Nt::IntRect>& right) {
			if (left.get().Right * left.get().Bottom == right.get().Right * right.get().Bottom)
				return (Double(left.get().Right) / Double(left.get().Bottom) < Double(right.get().Right) / Double(right.get().Bottom));

			return left.get().Right * left.get().Bottom > right.get().Right * right.get().Bottom;
			};

		textureAreas.sort(predicate);

		Int totalArea = 0;
		for (const Nt::IntRect& rect : textureAreas)
			totalArea += rect.Right * rect.Bottom;

		initialSize = Nt::Int2D().Fill((Int)std::ceil(std::sqrt(Double(totalArea))));

		std::vector<Nt::IntRect> freeAreas;
		freeAreas.push_back({ { }, initialSize });

		initialSize = { };
		for (std::list<std::reference_wrapper<Nt::IntRect>>::iterator rectIterator = textureAreas.begin(); rectIterator != textureAreas.end();) {
			std::vector<Nt::IntRect>::const_iterator bestFreeRectIterator =
				FindBestFit(freeAreas, *rectIterator);

			if (bestFreeRectIterator != freeAreas.end()) {
				PlaceRect(freeAreas, bestFreeRectIterator, *rectIterator);

				initialSize = initialSize.Max(rectIterator->get().LeftTop + rectIterator->get().RightBottom);

				++rectIterator;
			}
			else {
				ExpandAtlas(freeAreas, *rectIterator);
				++rectIterator;
			}
		}
	}
}