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

namespace deep {
	class RecurrentSparseAutoencoder {
	public:
		struct Experience {
			std::vector<float> _visibleStates;
			std::vector<float> _visibleStatesPrev;
			std::vector<float> _hiddenStatesPrev;
			std::vector<float> _hiddenStatesPrevPrev;
		};

	private:
		static float sigmoid(float x) {
			return 1.0f / (1.0f + std::exp(-x));
		}

		struct Connection {
			float _weight;
			float _trace;
			float _prevWeightDelta;

			Connection()
				: _trace(0.0f), _prevWeightDelta(0.0f)
			{}
		};

		struct HiddenNode {
			float _activation;
			float _activationPrev;
			float _state;
			float _statePrev;
			float _statePrevPrev;
			float _dutyCycle;

			Connection _bias;

			std::vector<Connection> _visibleHiddenConnections;

			std::vector<Connection> _hiddenHiddenConnections;

			HiddenNode()
				: _activation(0.0f), _state(0.0f), _statePrev(0.0f), _statePrevPrev(0.0f)
			{}
		};

		struct VisibleNode {
			float _state;
			float _statePrev;
			float _reconstruction;
			float _reconstructionPrev;

			Connection _bias;

			std::vector<Connection> _hiddenVisibleConnections;

			VisibleNode()
				: _state(0.0f), _statePrev(0.0f), _reconstruction(0.0f), _reconstructionPrev(0.0f)
			{}
		};

		std::vector<HiddenNode> _hiddenNodes;
		std::vector<VisibleNode> _visibleNodes;

	public:
		void createRandom(int numVisibleNodes, int numHiddenNodes, float sparsity, float minInitWeight, float maxInitWeight, float recurrentScalar, std::mt19937 &generator);

		void activate(float sparsity, float dutyCycleDecay);

		void learn(float sparsity, float stateLeak, float alpha, float beta, float gamma, float epsilon, float momentum, float traceDecay, float temperature);
		void learnExperience(const Experience &experience, float sparsity, float stateLeak, float alpha, float beta, float gamma, float epsilon, float momentum, float traceDecay, float temperature);

		void stepBegin();

		void clearMemory();

		void getCurrentExperience(Experience &experience) const;

		void setVisibleNodeState(int index, float value) {
			_visibleNodes[index]._state = value;
		}

		float getVisibleNodeState(int index) {
			return _visibleNodes[index]._state;
		}

		float getVisibleNodeReconstruction(int index) const {
			return _visibleNodes[index]._reconstruction;
		}

		float getHiddenNodeState(int index) const {
			return _hiddenNodes[index]._state;
		}

		void setHiddenNodeState(int index, float value) {
			_hiddenNodes[index]._state = value;
		}

		int getNumVisibleNodes() const {
			return _visibleNodes.size();
		}

		int getNumHiddenNodes() const {
			return _hiddenNodes.size();
		}

		friend class RSARL;
	};
}