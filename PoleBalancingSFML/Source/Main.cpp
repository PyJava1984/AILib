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

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <nn/QAgent.h>
#include <nn/ActorCriticAgent.h>
#include <nn/Cacla.h>
#include <nn/MemoryActor.h>
#include <nn/RLLSTMAgent.h>
#include <nn/PSOAgent.h>
#include <nn/MultiQ.h>

#include <lstm/LSTMActorCritic.h>
#include <lstm/LSTMGQ.h>

#include <deep/AutoLSTM.h>

#include <hypernet/BayesianOptimizerTrainer.h>
#include <hypernet/EvolutionaryTrainer.h>

#include <experiments/ExperimentPoleBalancing.h>
#include <experiments/ExperimentXOR.h>
#include <experiments/ExperimentOR.h>
#include <experiments/ExperimentAND.h>

#include <hypernet/SampleField.h>
#include <hypernet/BayesianOptimizer.h>

#include <dnf/Field.h>

#include <ctrnn/GeneticAlgorithm.h>

#include <falcon/Falcon.h>

#include <elman/ElmanNetwork.h>

#include <deep/RBM.h>

#include <time.h>
#include <iostream>
#include <random>
#include <array>
#include <fstream>

/*int main() {
	std::mt19937 generator(time(nullptr));*/
	
	/*hn::FunctionApproximator approx;
	approx.createRandom(2, 1, 1, 4, -0.2f, 0.2f, generator);

	float inputs[4][2] {
		{ 0.0f, 0.0f },
		{ 0.0f, 10.0f },
		{ 10.0f, 0.0f },
		{ 10.0f, 10.0f }
	};

	float outputs[4] {
		0.0f,
		0.0f,
		1.0f,
		0.0f
	};

	float reward = 0.0f;
	float prevReward = 0.0f;
	float totalReward = 0.0f;

	std::vector<float> inputBuffer(2);
	std::vector<float> outputBuffer(1);

	for (size_t i = 0; i < 1000; i++) {
		float newReward = 1.0f;

		for (size_t j = 0; j < 4; j++) {
			inputBuffer[0] = (inputs[j][0]);
			inputBuffer[1] = (inputs[j][1]);

			std::vector<std::vector<float>> layerOutputs;

			approx.process(inputBuffer, layerOutputs);

			newReward -= std::abs(outputs[j] - layerOutputs.back()[0]) * 0.25f;

			approx.backpropagate(inputBuffer, layerOutputs, std::vector<float>(1, outputs[j]), 0.1f);
		}

		prevReward = reward;
		reward = newReward;

		totalReward += newReward;
	}

	for (size_t j = 0; j < 4; j++) {
		inputBuffer[0] = (inputs[j][0]);
		inputBuffer[1] = (inputs[j][1]);

		approx.process(inputBuffer, outputBuffer);

		std::cout << outputBuffer[0] << std::endl;
	}

	system("pause");*/

	/*hn::EvolutionaryTrainer trainer;
	hn::Config config;
	
	trainer.create(config, 32, generator, 6.0f);

	trainer.addExperiment(std::shared_ptr<hn::Experiment>(new ExperimentXOR()));
	trainer.addExperiment(std::shared_ptr<hn::Experiment>(new ExperimentOR()));
	trainer.addExperiment(std::shared_ptr<hn::Experiment>(new ExperimentAND()));

	for (size_t g = 0; g < 10000; g++) {
		std::cout << "Evaluating generation " << (g + 1) << "." << std::endl;

		trainer.evaluate(config, generator);

		std::cout << "Reproducing generation " << (g + 1) << "." << std::endl;

		trainer.reproduce(config, generator);

		std::cout << "Saving best to \"hypernet1.txt\"" << std::endl;

		std::ofstream toFile("hypernet1.txt");

		trainer.writeBestToStream(toFile);

		toFile.close();

		std::cout << "Generation completed." << std::endl;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			break;
	}

	return 0;
}*/

/*float testFunc(float x) {
	return 0.2f * x * x * x * x + 0.5f * x * x * x - 1.2f * x * x + 0.2f * x + 0.3f;
}*/

/*int main() {*/
	/*sf::RenderWindow window;

	window.create(sf::VideoMode(800, 600), "Pole Balancing");

	window.setVerticalSyncEnabled(true);

	hn::SampleField sampleField;
	sampleField.create(2, 3);
	sampleField._kernel = std::bind(hn::kernelSquaredExponential, std::placeholders::_1, std::placeholders::_2, 128.0f);

	std::mt19937 generator(time(nullptr));

	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

	// Add some samples
	for (size_t i = 0; i < 100; i++) {
		hn::SampleField::Sample s;

		s._x.resize(2);
		s._x[0] = dist01(generator);
		s._x[1] = dist01(generator);
		s._y.resize(3);
		s._y[0] = dist01(generator);
		s._y[1] = dist01(generator);
		s._y[2] = dist01(generator);

		sampleField.addSample(s);
	}
	
	sf::Image image;

	image.create(800, 600);

	for (unsigned int x = 0; x < image.getSize().x; x++)
	for (unsigned int y = 0; y < image.getSize().y; y++) {
		std::vector<float> sx(2);
		sx[0] = static_cast<float>(x) / static_cast<float>(image.getSize().x);
		sx[1] = static_cast<float>(y) / static_cast<float>(image.getSize().y);

		std::vector<float> sy = sampleField.getYAtX(sx);

		sf::Color c;

		c.r = sy[0] * 255.0f;
		c.g = sy[1] * 255.0f;
		c.b = sy[2] * 255.0f;

		image.setPixel(x, y, c);
	}

	sf::Texture texture;
	texture.loadFromImage(image);

	// ----------------------------- Game Loop -----------------------------

	bool quit = false;

	sf::Clock clock;

	float dt = 0.017f;

	do {
		clock.restart();

		// ----------------------------- Input -----------------------------

		sf::Event windowEvent;

		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				quit = true;
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			quit = true;

		// ---------------------------- Rendering ----------------------------

		sf::Sprite s;
		s.setTexture(texture);

		window.draw(s);

		// -------------------------------------------------------------------

		window.display();

		dt = clock.getElapsedTime().asSeconds();
	} while (!quit);

	return 0;*/

	/*std::mt19937 generator(time(nullptr));
	hn::BayesianOptimizer bo;

	bo.create(1, std::vector<float>(1, -3.0f), std::vector<float>(1, 1.0f));

	for (size_t i = 0; i < 6; i++) {
		bo.generateNewVariables(generator);

		bo.update(testFunc(bo.getCurrentVariables()[0]));
	}

	std::cout << bo.getCurrentVariables()[0] << std::endl;

	system("pause");*/

	/*std::mt19937 generator(time(nullptr));

	hn::Config config;
	hn::BayesianOptimizerTrainer bot;

	bot.create(config, -2.0f, 2.0f, generator);

	bot.addExperiment(std::shared_ptr<hn::Experiment>(new ExperimentXOR()));
	bot.addExperiment(std::shared_ptr<hn::Experiment>(new ExperimentOR()));
	bot.addExperiment(std::shared_ptr<hn::Experiment>(new ExperimentAND()));

	for (size_t g = 0; g < 1000; g++) {
		bot.evaluate(config, generator);

		std::cout << "Current fitness: " << bot.getCurrentFitness() << std::endl;

		bot.update(config, generator);
	}

	return 0;
}*/

/*int main() {
	float inputs[4][2] {
		{ -10.0f, -10.0f },
		{ -10.0f, 10.0f },
		{ 10.0f, -10.0f },
		{ 10.0f, 10.0f }
	};

	float outputs[4] {
		0.0f,
		1.0f,
		1.0f,
		0.0f
	};

	hn::Config config;
	config._numInputGroups = 2;
	config._numInputsPerGroup = 1;
	config._numOutputGroups = 1;
	config._numOutputsPerGroup = 1;

	std::mt19937 generator(time(nullptr));
	hn::HyperNet hypernet;
	hypernet.readFromStream(std::ifstream("hypernet1.txt"));

	std::vector<int> dims(2);
	dims[0] = 12;
	dims[1] = 6;
	hypernet.generateFeedForward(config, 1, 4, generator);

	float reward = 0.0f;
	float prevReward = 0.0f;
	float totalReward = 0.0f;

	for (size_t i = 0; i < 400; i++) {
		float newReward = 1.0f;

		for (size_t j = 0; j < 4; j++) {
			hypernet.setInput(0, inputs[j][0]);
			hypernet.setInput(1, inputs[j][1]);

			hypernet.step(config, (reward - prevReward) * 50.0f, generator, 6, 6.0f);

			newReward -= std::abs(outputs[j] - hypernet.getOutput(0)) * 0.25f;
		}

		prevReward = reward;
		reward = newReward;

		totalReward += newReward;
	}

	for (size_t j = 0; j < 4; j++) {
		hypernet.setInput(0, inputs[j][0]);
		hypernet.setInput(1, inputs[j][1]);

		hypernet.step(config, (reward - prevReward) * 50.0f, generator, 6, 6.0f);

		std::cout << hypernet.getOutput(0) << std::endl;
	}

	std::cout << "totalReward: " << totalReward << std::endl;

	system("pause");

	return 0;
}*/

/*
int main() {
	std::mt19937 generator(time(nullptr));

	raahn::AutoEncoder ae;

	ae.create(10, 3, -0.3f, 0.3f, generator);

	std::vector<float> inputs(10, 0.0f);
	std::vector<float> outputs(4, 0.0f);

	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

	for (size_t j = 0; j < 10; j++) {
		inputs[j] = dist01(generator);
	}

	for (size_t i = 0; i < 1000; i++) {
		

		ae.update(inputs, outputs, 0.1f);

		float error = 0.0f;

		for (size_t j = 0; j < 10; j++)
			error += ae.getInputErrorBuffer()[j];

		std::cout << "Total Error: " << error << std::endl;

		
	}

	system("pause");

	return 0;
}*/

float evaluatePoleBalancing(ctrnn::CTRNN &net, std::mt19937 &generator) {
	float pixelsPerMeter = 128.0f;
	float poleLength = 1.0f;
	float g = -2.8f;
	float massMass = 20.0f;
	float cartMass = 2.0f;
	sf::Vector2f massPos(0.0f, poleLength);
	sf::Vector2f massVel(0.0f, 0.0f);
	float poleAngle = static_cast<float>(std::_Pi) * 0.0f;
	float poleAngleVel = 0.0f;
	float poleAngleAccel = 0.0f;
	float cartX = 0.0f;
	float cartVelX = 0.0f;
	float cartAccelX = 0.0f;
	float poleRotationalFriction = 0.008f;
	float cartMoveRadius = 1.8f;
	float cartFriction = 0.02f;
	float maxSpeed = 3.0f;

	float dt = 0.03f;

	float fitness = 0.0f;
	float prevFitness = 0.0f;

	float totalFitness = 0.0f;

	for (size_t i = 0; i < 1500; i++) {
		//std::cout << "Step " << i << std::endl;

		// Update fitness
		prevFitness = fitness;

		if (poleAngle < static_cast<float>(std::_Pi))
			fitness = -(static_cast<float>(std::_Pi) * 0.5f - poleAngle);
		else
			fitness = -(static_cast<float>(std::_Pi) * 0.5f - (static_cast<float>(std::_Pi) * 2.0f - poleAngle));

		//fitness = fitness - std::fabsf(poleAngleVel * 40.0f);

		totalFitness += fitness * 0.01f;

		// ------------------------------ AI -------------------------------

		float dFitness = fitness - prevFitness;

		float error = dFitness * 10.0f;

		//agent.reinforceArp(std::min(1.0f, std::max(-1.0f, error)) * 0.5f + 0.5f, 0.1f, 0.05f);

		net.setInput(0, cartX * 0.25f);
		net.setInput(1, cartVelX);
		net.setInput(2, std::fmodf(poleAngle + static_cast<float>(std::_Pi), 2.0f * static_cast<float>(std::_Pi)));
		net.setInput(3, poleAngleVel);

		net.setInput(4, dFitness * 0.1f);

		for (size_t s = 0; s < 18; s++)
			net.step(1.0f, dFitness * 0.1f, 0.1f, generator);

		float dir = std::min(1.0f, std::max(-1.0f, 2.0f * net.getOutput(0) - 1.0f));

		//dir = 1.4f * (dir * 2.0f - 1.0f);

		float agentForce = 4000.0f * dir;
		//float agentForce = 2000.0f * agent.getOutput(0);

		// ---------------------------- Physics ----------------------------

		float pendulumCartAccelX = cartAccelX;

		if (cartX < -cartMoveRadius)
			pendulumCartAccelX = 0.0f;
		else if (cartX > cartMoveRadius)
			pendulumCartAccelX = 0.0f;

		poleAngleAccel = pendulumCartAccelX * std::cosf(poleAngle) + g * std::sinf(poleAngle);
		poleAngleVel += -poleRotationalFriction * poleAngleVel + poleAngleAccel * dt;
		poleAngle += poleAngleVel * dt;

		massPos = sf::Vector2f(cartX + std::cosf(poleAngle + static_cast<float>(std::_Pi) * 0.5f) * poleLength, std::sinf(poleAngle + static_cast<float>(std::_Pi) * 0.5f) * poleLength);

		float force = 0.0f;

		if (std::fabsf(cartVelX) < maxSpeed)
			force = std::max(-4000.0f, std::min(4000.0f, agentForce));

		if (cartX < -cartMoveRadius) {
			cartX = -cartMoveRadius;

			cartAccelX = -cartVelX / dt;
			cartVelX = -0.5f * cartVelX;
		}
		else if (cartX > cartMoveRadius) {
			cartX = cartMoveRadius;

			cartAccelX = -cartVelX / dt;
			cartVelX = -0.5f * cartVelX;
		}

		cartAccelX = 0.25f * (force + massMass * poleLength * poleAngleAccel * std::cosf(poleAngle) - massMass * poleLength * poleAngleVel * poleAngleVel * std::sinf(poleAngle)) / (massMass + cartMass);
		cartVelX += -cartFriction * cartVelX + cartAccelX * dt;
		cartX += cartVelX * dt;

		poleAngle = std::fmodf(poleAngle, (2.0f * static_cast<float>(std::_Pi)));

		if (poleAngle < 0.0f)
			poleAngle += static_cast<float>(std::_Pi) * 2.0f;
	}

	//std::cout << "Pole balancing experiment finished with total fitness of " << totalFitness << "." << std::endl;

	return totalFitness;
}

float evaluateAND(ctrnn::CTRNN &net, std::mt19937 &generator) {
	float inputs[4][2] {
		{ -1.0f, -1.0f },
		{ -1.0f, 1.0f },
		{ 1.0f, -1.0f },
		{ 1.0f, 1.0f }
	};

	float outputs[4] {
		0.0f,
		0.0f,
		0.0f,
		1.0f
	};

	float reward = 0.0f;
	float prevReward = 0.0f;

	float initReward = 0.0f;

	float totalReward = 0.0f;

	std::vector<float> values(4);

	for (size_t i = 0; i < 100; i++) {
		float newReward = 1.0f;

		float average = 0.0f;

		for (size_t j = 0; j < 4; j++) {
			net.setInput(0, inputs[j][0]);
			net.setInput(1, inputs[j][1]);
			net.setInput(2, inputs[j][0]);
			net.setInput(3, inputs[j][1]);
			net.setInput(4, (reward - prevReward) * 0.1f);

			for(size_t s = 0; s < 18; s++)
				net.step(1.0f, (reward - prevReward) * 0.1f, 0.1f, generator);

			newReward -= std::pow(std::abs(outputs[j] - net.getOutput(0)), 1.0f) * 0.25f;

			values[j] = net.getOutput(0);
			average += values[j];
		}

		average *= 0.25f;

		//for (size_t j = 0; j < 4; j++) {
		//	newReward -= 0.05f / (0.125f + 8.0f * std::abs(average - values[j]));
		//}

		prevReward = reward;
		reward = newReward;

		if (i == 0)
			initReward = std::min(1.0f, std::max(0.0f, reward));

		totalReward += reward * 0.1f;
	}

	//std::cout << "Finished AND experiment with total reward of " << (reward - initReward) * std::min(1.0f, std::max(0.0f, (reward * reward))) << "." << std::endl;

	return totalReward;
}

float evaluateOR(ctrnn::CTRNN &net, std::mt19937 &generator) {
	float inputs[4][2] {
		{ -1.0f, -1.0f },
		{ -1.0f, 1.0f },
		{ 1.0f, -1.0f },
		{ 1.0f, 1.0f }
	};

	float outputs[4] {
		0.0f,
		1.0f,
		1.0f,
		1.0f
	};

	float reward = 0.0f;
	float prevReward = 0.0f;

	float initReward = 0.0f;

	float totalReward = 0.0f;

	std::vector<float> values(4);

	for (size_t i = 0; i < 100; i++) {
		float newReward = 1.0f;

		float average = 0.0f;

		for (size_t j = 0; j < 4; j++) {
			net.setInput(0, inputs[j][0]);
			net.setInput(1, inputs[j][1]);
			net.setInput(2, inputs[j][0]);
			net.setInput(3, inputs[j][1]);
			net.setInput(4, (reward - prevReward) * 0.1f);

			for (size_t s = 0; s < 18; s++)
				net.step(1.0f, (reward - prevReward) * 0.1f, 0.1f, generator);

			newReward -= std::pow(std::abs(outputs[j] - net.getOutput(0)), 1.0f) * 0.25f;

			values[j] = net.getOutput(0);
			average += values[j];
		}

		average *= 0.25f;

		//for (size_t j = 0; j < 4; j++) {
		//	newReward -= 0.05f / (0.125f + 8.0f * std::abs(average - values[j]));
		//}

		prevReward = reward;
		reward = newReward;

		if (i == 0)
			initReward = std::min(1.0f, std::max(0.0f, reward));

		totalReward += reward * 0.5f;
	}

	//std::cout << "Finished AND experiment with total reward of " << (reward - initReward) * std::min(1.0f, std::max(0.0f, (reward * reward))) << "." << std::endl;

	return totalReward;
}

float evaluateXOR(ctrnn::CTRNN &net, std::mt19937 &generator) {
	float inputs[4][2] {
		{ -1.0f, -1.0f },
		{ -1.0f, 1.0f },
		{ 1.0f, -1.0f },
		{ 1.0f, 1.0f }
	};

	float outputs[4] {
		0.0f,
		1.0f,
		1.0f,
		0.0f
	};

	float reward = 0.0f;
	float prevReward = 0.0f;

	float initReward = 0.0f;

	float totalReward = 0.0f;

	std::vector<float> values(4);

	for (size_t i = 0; i < 100; i++) {
		float newReward = 1.0f;

		float average = 0.0f;

		for (size_t j = 0; j < 4; j++) {
			net.setInput(0, inputs[j][0]);
			net.setInput(1, inputs[j][1]);
			net.setInput(2, inputs[j][0]);
			net.setInput(3, inputs[j][1]);
			net.setInput(4, (reward - prevReward) * 0.1f);

			for (size_t s = 0; s < 18; s++)
				net.step(1.0f, (reward - prevReward) * 0.1f, 0.1f, generator);

			newReward -= std::pow(std::abs(outputs[j] - net.getOutput(0)), 1.0f) * 0.25f;

			values[j] = net.getOutput(0);
			average += values[j];
		}

		average *= 0.25f;

		//for (size_t j = 0; j < 4; j++) {
		//	newReward -= 0.05f / (0.125f + 8.0f * std::abs(average - values[j]));
		//}

		prevReward = reward;
		reward = newReward;

		if (i == 0)
			initReward = std::min(1.0f, std::max(0.0f, reward));

		totalReward += reward * 0.5f;
	}

	//std::cout << "Finished AND experiment with total reward of " << (reward - initReward) * std::min(1.0f, std::max(0.0f, (reward * reward))) << "." << std::endl;

	return totalReward;
}

/*int main() {
	std::mt19937 generator(time(nullptr));

	ctrnn::GeneticAlgorithm ga;

	ga.create(40, 5, 1, 14, -0.5f, 0.5f, 1.01f, 2.0f, 0.05f, 1.0f, generator);

	for (size_t g = 0; g < 550; g++) {
		float maxFitness = -99999.0f;

		for (size_t i = 0; i < ga.getPopulationSize(); i++) {
			//ctrnn::CTRNN memberPoleBalancing = ga.getPopulationMember(i);
			ctrnn::CTRNN memberAND = ga.getPopulationMember(i);
			ctrnn::CTRNN memberOR = ga.getPopulationMember(i);
			ctrnn::CTRNN memberXOR = ga.getPopulationMember(i);

			ga.setFitness(i, evaluateAND(memberAND, generator) + evaluateOR(memberOR, generator) + evaluateXOR(memberXOR, generator));

			if (ga.getFitness(i) > maxFitness)
				maxFitness = ga.getFitness(i);
		}

		ga.generation(0.3f, 0.05f, 0.2f, 0.3f, 0.05f, 0.2f, 0.3f, 0.05f, 0.2f, 3.0f, 4, generator);

		std::cout << "Finished generation " << g << ". Peak fitness: " << maxFitness << std::endl;
	}

	float maxFitness = -99999.0f;
	size_t maxFitnessIndex = 0;

	for (size_t i = 0; i < ga.getPopulationSize(); i++) {
		//ctrnn::CTRNN memberPoleBalancing = ga.getPopulationMember(i);
		ctrnn::CTRNN memberAND = ga.getPopulationMember(i);
		ctrnn::CTRNN memberOR = ga.getPopulationMember(i);
		ctrnn::CTRNN memberXOR = ga.getPopulationMember(i);

		float f = evaluateAND(memberAND, generator) + evaluateOR(memberOR, generator) + evaluateXOR(memberXOR, generator);

		if (f > maxFitness) {
			maxFitness = f;
			maxFitnessIndex = i;
		}
	}

	ctrnn::CTRNN best = ga.getPopulationMember(maxFitnessIndex);

	best.clear();

	float inputs[4][2] {
		{ -1.0f, -1.0f },
		{ -1.0f, 1.0f },
		{ 1.0f, -1.0f },
		{ 1.0f, 1.0f }
	};

	float outputs[4] {
		0.0f,
		1.0f,
		1.0f,
		0.0f
	};

	float reward = 0.0f;
	float prevReward = 0.0f;

	float initReward = 0.0f;

	float totalReward = 0.0f;

	std::vector<float> values(4);

	for (size_t i = 0; i < 100; i++) {
		float newReward = 1.0f;

		float average = 0.0f;

		for (size_t j = 0; j < 4; j++) {
			best.setInput(0, inputs[j][0]);
			best.setInput(1, inputs[j][1]);
			best.setInput(2, inputs[j][0]);
			best.setInput(3, inputs[j][1]);
			best.setInput(4, (reward - prevReward) * 0.1f);

			for (size_t s = 0; s < 16; s++)
				best.step(1.0f, (reward - prevReward) * 0.1f, 0.1f, generator);

			newReward -= std::pow(std::abs(outputs[j] - best.getOutput(0)), 2.0f) * 0.25f;

			if (i == 49)
				std::cout << best.getOutput(0) << std::endl;

			values[j] = best.getOutput(0);
			average += values[j];
		}

		average *= 0.25f;

		//for (size_t j = 0; j < 4; j++) {
		//	newReward -= 0.05f / (0.125f + 8.0f * std::abs(average - values[j]));
		//}

		prevReward = reward;
		reward = newReward;

		if (i == 0)
			initReward = std::min(1.0f, std::max(0.0f, reward));

		totalReward += reward * 0.5f;
	}

	best.clear();

	sf::RenderWindow window;

	window.create(sf::VideoMode(800, 600), "Pole Balancing");

	window.setVerticalSyncEnabled(true);

	//window.setFramerateLimit(60);

	// -------------------------- Load Resources --------------------------

	sf::Texture backgroundTexture;
	sf::Texture cartTexture;
	sf::Texture poleTexture;

	backgroundTexture.loadFromFile("Resources/background.png");
	cartTexture.loadFromFile("Resources/cart.png");
	poleTexture.loadFromFile("Resources/pole.png");

	// --------------------------------------------------------------------

	sf::Sprite backgroundSprite;
	sf::Sprite cartSprite;
	sf::Sprite poleSprite;

	backgroundSprite.setTexture(backgroundTexture);
	cartSprite.setTexture(cartTexture);
	poleSprite.setTexture(poleTexture);

	backgroundSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	cartSprite.setOrigin(sf::Vector2f(static_cast<float>(cartSprite.getTexture()->getSize().x) * 0.5f, static_cast<float>(cartSprite.getTexture()->getSize().y)));
	poleSprite.setOrigin(sf::Vector2f(static_cast<float>(poleSprite.getTexture()->getSize().x) * 0.5f, static_cast<float>(poleSprite.getTexture()->getSize().y)));

	// ----------------------------- Physics ------------------------------

	float pixelsPerMeter = 128.0f;
	float poleLength = 1.0f;
	float g = -2.8f;
	float massMass = 20.0f;
	float cartMass = 2.0f;
	sf::Vector2f massPos(0.0f, poleLength);
	sf::Vector2f massVel(0.0f, 0.0f);
	float poleAngle = static_cast<float>(std::_Pi) * 0.0f;
	float poleAngleVel = 0.0f;
	float poleAngleAccel = 0.0f;
	float cartX = 0.0f;
	float cartVelX = 0.0f;
	float cartAccelX = 0.0f;
	float poleRotationalFriction = 0.008f;
	float cartMoveRadius = 1.8f;
	float cartFriction = 0.02f;
	float maxSpeed = 3.0f;

	// ---------------------------- Game Loop -----------------------------

	bool quit = false;

	sf::Clock clock;

	float dt = 0.017f;

	float fitness = 0.0f;
	
	float lowPassFitness = 0.0f;

	bool reverseDirection = false;

	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

	do {
		clock.restart();

		// ----------------------------- Input -----------------------------

		sf::Event windowEvent;

		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				quit = true;
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			quit = true;

		// Update fitness
		if (poleAngle < static_cast<float>(std::_Pi))
			fitness = -(static_cast<float>(std::_Pi) * 0.5f - poleAngle);
		else
			fitness = -(static_cast<float>(std::_Pi) * 0.5f - (static_cast<float>(std::_Pi) * 2.0f - poleAngle));

		//fitness = fitness - std::fabsf(poleAngleVel * 40.0f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			fitness = -cartX;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			fitness = cartX;

		// ------------------------------ AI -------------------------------

		float dFitness = fitness - lowPassFitness;

		float error = dFitness * 10.0f;

		//agent.reinforceArp(std::min(1.0f, std::max(-1.0f, error)) * 0.5f + 0.5f, 0.1f, 0.05f);

		best.setInput(0, cartX * 0.25f);
		best.setInput(1, cartVelX);
		best.setInput(2, std::fmodf(poleAngle + static_cast<float>(std::_Pi), 2.0f * static_cast<float>(std::_Pi)));
		best.setInput(3, poleAngleVel);

		best.setInput(4, dFitness * 10.0f);

		for (size_t s = 0; s < 16; s++)
			best.step(1.0f, dFitness, 0.1f, generator);

		lowPassFitness += (fitness - lowPassFitness) * 0.2f;

		//std::cout << dFitness * 0.5f - 0.001f << std::endl;

		float dir = std::min(1.0f, std::max(-1.0f, 2.0f * best.getOutput(0) - 1.0f));

		//dir = 1.4f * (dir * 2.0f - 1.0f);

		float agentForce = 4000.0f * dir;
		//float agentForce = 2000.0f * agent.getOutput(0);

		// ---------------------------- Physics ----------------------------

		float pendulumCartAccelX = cartAccelX;

		if (cartX < -cartMoveRadius)
			pendulumCartAccelX = 0.0f;
		else if (cartX > cartMoveRadius)
			pendulumCartAccelX = 0.0f;

		poleAngleAccel = pendulumCartAccelX * std::cosf(poleAngle) + g * std::sinf(poleAngle);
		poleAngleVel += -poleRotationalFriction * poleAngleVel + poleAngleAccel * dt;
		poleAngle += poleAngleVel * dt;

		massPos = sf::Vector2f(cartX + std::cosf(poleAngle + static_cast<float>(std::_Pi) * 0.5f) * poleLength, std::sinf(poleAngle + static_cast<float>(std::_Pi) * 0.5f) * poleLength);

		float force = 0.0f;

		if (std::fabsf(cartVelX) < maxSpeed) {
			force = std::max(-4000.0f, std::min(4000.0f, agentForce));

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				force = -4000.0f;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				force = 4000.0f;
		}

		if (cartX < -cartMoveRadius) {
			cartX = -cartMoveRadius;

			cartAccelX = -cartVelX / dt;
			cartVelX = -0.5f * cartVelX;
		}
		else if (cartX > cartMoveRadius) {
			cartX = cartMoveRadius;

			cartAccelX = -cartVelX / dt;
			cartVelX = -0.5f * cartVelX;
		}

		cartAccelX = 0.25f * (force + massMass * poleLength * poleAngleAccel * std::cosf(poleAngle) - massMass * poleLength * poleAngleVel * poleAngleVel * std::sinf(poleAngle)) / (massMass + cartMass);
		cartVelX += -cartFriction * cartVelX + cartAccelX * dt;
		cartX += cartVelX * dt;

		poleAngle = std::fmodf(poleAngle, (2.0f * static_cast<float>(std::_Pi)));

		if (poleAngle < 0.0f)
			poleAngle += static_cast<float>(std::_Pi) * 2.0f;

		// ---------------------------- Rendering ----------------------------

		window.clear();

		window.draw(backgroundSprite);

		cartSprite.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) * 0.5f + pixelsPerMeter * cartX, static_cast<float>(window.getSize().y) * 0.5f + 3.0f));

		window.draw(cartSprite);

		poleSprite.setPosition(cartSprite.getPosition() + sf::Vector2f(0.0f, -45.0f));
		poleSprite.setRotation(poleAngle * 180.0f / static_cast<float>(std::_Pi) + 180.0f);

		window.draw(poleSprite);

		// -------------------------------------------------------------------

		window.display();

		dt = clock.getElapsedTime().asSeconds();
	} while (!quit);
}*/

/*int main() {
	std::mt19937 generator(time(nullptr));

	sf::RenderWindow window;
	window.create(sf::VideoMode(512, 512), "AILib", sf::Style::Default);

	dnf::Field field;
	field.createRandom(2, 32, 2, 0.75f, 0.0f, 1.0f, generator);

	float size = std::min(window.getSize().x / static_cast<float>(field.getSize()), window.getSize().y / static_cast<float>(field.getSize()));

	sf::Vector2f boxSize(size, size);

	// -------------------------- Simulation Loop -------------------------

	bool quit = false;

	sf::Clock clock;

	float dt = 0.017f;

	float switchPatternTimer = 0.0f;
	float switchPatternTime = 180.0f;

	int pattern = 0;

	float inputPattern[4][2] = {
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f }
	};

	float outputPattern[4] = {
		0.0f,
		1.0f,
		1.0f,
		0.0f
	};

	float reward = 0.0f;
	float prevReward = 0.0f;

	float averageReward = 0.0f;

	float fieldAverage = 0.0f;

	do {
		clock.restart();

		// ----------------------------- Input -----------------------------

		sf::Event windowEvent;

		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				quit = true;
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			quit = true;

		std::vector<float> input(4);

		input[0] = inputPattern[pattern][0];
		input[1] = inputPattern[pattern][1];

		// -------------------------------------------------------------------

		window.clear();

		field.step(input, 0.2f, 0.0f, 2.0f, (reward - prevReward) * 0.005f, 0.05f, 0.005f, 0.005f, 0.005f, 0.3f);

		// Draw grid
		int lIndex = 0;

		for (int x = 0; x < field.getSize(); x++)
		for (int y = 0; y < field.getSize(); y++) {
			sf::RectangleShape rectShape;

			rectShape.setPosition(x * boxSize.x, y * boxSize.y);
			rectShape.setSize(boxSize);

			float positivePotentialColor = field.getOutputE(x, y);
			float negativePotentialColor = field.getOutputE(x, y);

			rectShape.setFillColor(sf::Color(positivePotentialColor * 255.0f, negativePotentialColor * 255.0f, 0));

			window.draw(rectShape);

			lIndex++;
		}

		switchPatternTimer += 1.0f;

		if (switchPatternTimer > switchPatternTime) {
			prevReward = reward;

			reward = -std::pow(std::abs(outputPattern[pattern] - field.getOutputE(16, 16)), 2.0f);

			averageReward += (reward - averageReward) * 0.1f;

			std::cout << field.getOutputE(16, 16) << "    " << averageReward << "     " << reward << std::endl;

			switchPatternTimer -= switchPatternTime;

			pattern = (pattern + 1) % 4;

			fieldAverage = 0.0f;
		}

		// -------------------------------------------------------------------

		window.display();

		dt = clock.getElapsedTime().asSeconds();
	} while (!quit);

	return 0;
}*/

int main() {
	std::mt19937 generator(time(nullptr));

	float reward = 0.0f;
	float prevReward = 0.0f;

	float initReward = 0.0f;

	float totalReward = 0.0f;

	sf::RenderWindow window;

	window.create(sf::VideoMode(800, 600), "Pole Balancing");

	window.setVerticalSyncEnabled(true);

	//window.setFramerateLimit(60);

	// -------------------------- Load Resources --------------------------

	sf::Texture backgroundTexture;
	sf::Texture cartTexture;
	sf::Texture poleTexture;

	backgroundTexture.loadFromFile("Resources/background.png");
	cartTexture.loadFromFile("Resources/cart.png");
	poleTexture.loadFromFile("Resources/pole.png");

	// --------------------------------------------------------------------

	sf::Sprite backgroundSprite;
	sf::Sprite cartSprite;
	sf::Sprite poleSprite;

	backgroundSprite.setTexture(backgroundTexture);
	cartSprite.setTexture(cartTexture);
	poleSprite.setTexture(poleTexture);

	backgroundSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	cartSprite.setOrigin(sf::Vector2f(static_cast<float>(cartSprite.getTexture()->getSize().x) * 0.5f, static_cast<float>(cartSprite.getTexture()->getSize().y)));
	poleSprite.setOrigin(sf::Vector2f(static_cast<float>(poleSprite.getTexture()->getSize().x) * 0.5f, static_cast<float>(poleSprite.getTexture()->getSize().y)));

	// ----------------------------- Physics ------------------------------

	float pixelsPerMeter = 128.0f;
	float poleLength = 1.0f;
	float g = -2.8f;
	float massMass = 20.0f;
	float cartMass = 2.0f;
	sf::Vector2f massPos(0.0f, poleLength);
	sf::Vector2f massVel(0.0f, 0.0f);
	float poleAngle = static_cast<float>(std::_Pi) * 0.0f;
	float poleAngleVel = 0.0f;
	float poleAngleAccel = 0.0f;
	float cartX = 0.0f;
	float cartVelX = 0.0f;
	float cartAccelX = 0.0f;
	float poleRotationalFriction = 0.008f;
	float cartMoveRadius = 1.8f;
	float cartFriction = 0.02f;
	float maxSpeed = 3.0f;

	// ---------------------------- Game Loop -----------------------------

	bool quit = false;

	sf::Clock clock;

	float dt = 0.017f;

	float fitness = 0.0f;
	float prevFitness = 0.0f;

	float lowPassFitness = 0.0f;

	bool reverseDirection = false;

	bool trainMode = true;

	bool tDownLastFrame = false;

	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

	//deep::AutoLSTM alstm;

	//elman::ElmanNetwork en;

	//en.createRandom(4, 1, 10, -0.5f, 0.5f, generator);

	lstm::LSTMActorCritic lstmAC;

	lstmAC.createRandom(4, 1, 1, 10, 2, 1, 1, 10, 2, 1, -0.5f, 0.5f, generator);

	falcon::Falcon fal;
	fal.create(4, 1);

	std::array<falcon::Falcon::FieldParams, 3> fieldParams;

	fieldParams[0]._baseVigilance = 0.2f;
	fieldParams[1]._baseVigilance = 0.3f;
	fieldParams[2]._baseVigilance = 0.1f;

	fieldParams[0]._alpha = 0.1f;
	fieldParams[1]._alpha = 0.01f;
	fieldParams[2]._alpha = 0.01f;

	fieldParams[0]._gamma = 0.5f;
	fieldParams[1]._gamma = 0.5f;
	fieldParams[2]._gamma = 0.2f;

	sf::Font font;

	font.loadFromFile("Resources/pixelated.ttf");

	do {
		clock.restart();

		// ----------------------------- Input -----------------------------

		sf::Event windowEvent;

		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				quit = true;
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			quit = true;

		// Update fitness
		if (poleAngle < static_cast<float>(std::_Pi))
			fitness = -(static_cast<float>(std::_Pi) * 0.5f - poleAngle);
		else
			fitness = -(static_cast<float>(std::_Pi) * 0.5f - (static_cast<float>(std::_Pi) * 2.0f - poleAngle));

		//fitness = fitness - std::fabsf(poleAngleVel * 40.0f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			fitness = -cartX;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			fitness = cartX;

		// ------------------------------ AI -------------------------------

		float dFitness = fitness - prevFitness;

		float error = dFitness * 5.0f;

		//agent.reinforceArp(std::min(1.0f, std::max(-1.0f, error)) * 0.5f + 0.5f, 0.1f, 0.05f);

		lstmAC.setInput(0, cartX * 0.25f);
		lstmAC.setInput(1, cartVelX);
		lstmAC.setInput(2, std::fmodf(poleAngle + static_cast<float>(std::_Pi), 2.0f * static_cast<float>(std::_Pi)));
		lstmAC.setInput(3, poleAngleVel);

		lstmAC.step(dFitness * 50.0f, 0.001f, 0.04f, 0.001f, 0.9f, 0.0f, 0.5f, 0.6f, 0.2f, 0.2f, 0.75f, generator);

		//fal.update(fitness, 0.1f, 0.95f, 0.05f, fieldParams, 0.5f, 0.95f, 0.05f, generator);

		lowPassFitness += (fitness - lowPassFitness) * 0.2f;

		float agentForce = 0.0f;

		//if (!trainMode) {
		float dir = std::min(1.0f, std::max(-1.0f, lstmAC.getOutput(0)));

			//dir = 1.4f * (dir * 2.0f - 1.0f);

			agentForce = 4000.0f * dir;
		//}
		//float agentForce = 2000.0f * agent.getOutput(0);

			prevFitness = fitness;

		// ---------------------------- Physics ----------------------------

		float pendulumCartAccelX = cartAccelX;

		if (cartX < -cartMoveRadius)
			pendulumCartAccelX = 0.0f;
		else if (cartX > cartMoveRadius)
			pendulumCartAccelX = 0.0f;

		poleAngleAccel = pendulumCartAccelX * std::cosf(poleAngle) + g * std::sinf(poleAngle);
		poleAngleVel += -poleRotationalFriction * poleAngleVel + poleAngleAccel * dt;
		poleAngle += poleAngleVel * dt;

		massPos = sf::Vector2f(cartX + std::cosf(poleAngle + static_cast<float>(std::_Pi) * 0.5f) * poleLength, std::sinf(poleAngle + static_cast<float>(std::_Pi) * 0.5f) * poleLength);

		float force = 0.0f;

		if (std::fabsf(cartVelX) < maxSpeed) {
			force = std::max(-4000.0f, std::min(4000.0f, agentForce));

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				force = -4000.0f;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				force = 4000.0f;
		}

		//if (trainMode) {
		//	en.calculateGradient(std::vector<float>(1, force / 4000.0f));

		//	en.moveAlongGradient(0.01f);
		//}

		//en.updateContext();

		if (cartX < -cartMoveRadius) {
			cartX = -cartMoveRadius;

			cartAccelX = -cartVelX / dt;
			cartVelX = -0.5f * cartVelX;
		}
		else if (cartX > cartMoveRadius) {
			cartX = cartMoveRadius;

			cartAccelX = -cartVelX / dt;
			cartVelX = -0.5f * cartVelX;
		}

		cartAccelX = 0.25f * (force + massMass * poleLength * poleAngleAccel * std::cosf(poleAngle) - massMass * poleLength * poleAngleVel * poleAngleVel * std::sinf(poleAngle)) / (massMass + cartMass);
		cartVelX += -cartFriction * cartVelX + cartAccelX * dt;
		cartX += cartVelX * dt;

		poleAngle = std::fmodf(poleAngle, (2.0f * static_cast<float>(std::_Pi)));

		if (poleAngle < 0.0f)
			poleAngle += static_cast<float>(std::_Pi) * 2.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
			if (!tDownLastFrame) {
				trainMode = !trainMode;
			}
			
			tDownLastFrame = true;
		}
		else
			tDownLastFrame = false;

		// ---------------------------- Rendering ----------------------------

		window.clear();

		window.draw(backgroundSprite);

		cartSprite.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) * 0.5f + pixelsPerMeter * cartX, static_cast<float>(window.getSize().y) * 0.5f + 3.0f));

		window.draw(cartSprite);

		poleSprite.setPosition(cartSprite.getPosition() + sf::Vector2f(0.0f, -45.0f));
		poleSprite.setRotation(poleAngle * 180.0f / static_cast<float>(std::_Pi) + 180.0f);

		window.draw(poleSprite);

		if (trainMode) {
			sf::Text text;

			text.setFont(font);

			text.setString("Train");

			text.setPosition(10.0f, 10.0f);

			text.setColor(sf::Color::Blue);

			window.draw(text);
		}

		// -------------------------------------------------------------------

		window.display();

		dt = clock.getElapsedTime().asSeconds();
	} while (!quit);
}