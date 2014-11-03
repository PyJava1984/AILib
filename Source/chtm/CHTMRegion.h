/*
AI Lib
Copyright (C) 2014 Eric Laukien

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <vector>
#include <random>

namespace chtm {
	class CHTMRegion {
	public:
		struct LateralConnection {
			float _weight;
		};

		struct InputConnection {
			float _weight;
		};

		struct OutputConnection {
			float _weight;

			float _eligibility;

			OutputConnection()
				: _eligibility(0.0f)
			{}
		};

		struct ColumnCell {
			float _state;
			float _prediction;
			float _predictionPrev;

			std::vector<LateralConnection> _connections;

			ColumnCell()
				: _state(0.0f), _prediction(0.0f), _predictionPrev(0.0f)
			{}
		};

		struct Column {
			std::vector<InputConnection> _center;

			float _width;

			float _activation;
			float _state;
			float _prediction;
			float _predictionPrev;
			float _output;

			std::vector<ColumnCell> _cells;

			Column()
				: _activation(0.0f), _state(0.0f), _prediction(0.0f), _predictionPrev(0.0f), _output(0.0f)
			{}
		};

		struct OutputNode {
			std::vector<OutputConnection> _connections;

			OutputConnection _bias;
		};

		static float sigmoid(float x) {
			return 1.0f / (1.0f + std::exp(-x));
		}

	private:
		std::vector<Column> _columns;
		std::vector<OutputNode> _outputNodes;

		int _inputWidth, _inputHeight;
		int _columnsWidth, _columnsHeight;
		int _cellsPerColumn;
		int _receptiveRadius;
		int _cellRadius;

	public:
		void createRandom(int inputWidth, int inputHeight, int columnsWidth, int columnsHeight, int cellsPerColumn, int receptiveRadius, int cellRadius, int numOutputs,
			float minCenter, float maxCenter, float minWidth, float maxWidth, float minInputWeight, float maxInputWeight,
			float minCellWeight, float maxCellWeight, float minOutputWeight, float maxOutputWeight, std::mt19937 &generator);

		void stepBegin();

		void getOutput(const std::vector<float> &input, std::vector<float> &output, int inhibitionRadius, float sparsity, float cellIntensity, float predictionIntensity, std::mt19937 &generator);
	
		void learn(const std::vector<float> &input, const std::vector<float> &output, const std::vector<float> &target, float weightAlpha, float centerAlpha, float widthAlpha, float widthScalar, float minDistance, float minLearningThreshold, float cellAlpha);

		void learnTraces(const std::vector<float> &input, const std::vector<float> &output, const std::vector<float> &error, const std::vector<float> &outputWeightAlphas, float centerAlpha, float widthAlpha, float widthScalar, float minDistance, float minLearningThreshold, float cellAlpha, const std::vector<float> &outputLambdas);

		void findInputError(const std::vector<float> &input, const std::vector<float> &output, const std::vector<float> &target, std::vector<float> &inputError);

		int getNumOutputs() const {
			return _outputNodes.size();
		}

		int getNumColumns() const {
			return _columns.size();
		}

		const Column &getColumn(int x, int y) const {
			return _columns[x + y * _columnsWidth];
		}
	};
}