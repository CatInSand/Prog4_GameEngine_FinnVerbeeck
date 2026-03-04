#ifndef PLOTTER_H
#define PLOTTER_H

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "imgui_plot.h"

#include <vector>
#include <algorithm>
#include <chrono>

namespace dae
{
	class Plotter
	{
	public:
		Plotter() = default;

		bool IsReady() { return m_xValues.size() > 0; }
		void DrawPlots()
		{
			ImGui::PlotConfig conf{};
			conf.values.xs = m_xValues.data();
			conf.values.ys = m_yValues.data();
			conf.values.count = static_cast<int32_t>(m_yValues.size());
			conf.values.color = 0xFF'00'7F'FF;
			conf.scale.min = 0;
			conf.scale.max = *std::max_element(m_yValues.begin(), m_yValues.end());
			conf.tooltip.show = true;
			conf.tooltip.format = "x=%.0f, y=%.5f";
			conf.grid_x.show = true;
			conf.grid_y.show = true;
			conf.frame_size = ImVec2(200, 100);
			conf.line_thickness = 2.f;

			ImGui::Plot("results", conf);
		}
		void CalculatePlots()
		{
			std::vector<std::vector<float>> m_Samples{};
			static int sampleCount{ 10 };

			for (int index{ 0 }; index < sampleCount; ++index)
			{
				m_xValues.clear();
				m_yValues.clear();

				std::vector<std::pair<long, int>> durations{};
				std::chrono::high_resolution_clock::time_point startTime{};

				std::vector<int> arr(100000);

				for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
				{
					startTime = std::chrono::high_resolution_clock::now();

					for (int i{ 0 }; i < arr.size(); i += stepSize)
					{
						arr[i] *= 2;
					}

					durations.push_back(std::pair<long, int>{ static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count()), stepSize });
				}

				for (std::pair<long, int>& duration : durations)
				{
					m_yValues.push_back(static_cast<float>(duration.first));
					m_xValues.push_back(static_cast<float>(duration.second));
				}

				m_Samples.push_back(std::move(m_yValues));
			}

			m_yValues.clear();

			for (int iteration{ 0 }; iteration < m_xValues.size(); ++iteration)
			{
				//drop lowest and highest
				std::vector<float> samples{};
				for (int index{ 0 }; index < sampleCount; ++index)
				{
					samples.push_back(m_Samples[index][iteration]);
				}
				samples.erase(std::max_element(samples.begin(), samples.end()));
				samples.erase(std::min_element(samples.begin(), samples.end()));

				//take average
				float total{};
				for (float sample : samples)
				{
					total += sample;
				}
				m_yValues.push_back(total / (sampleCount - 2));
			}
		}
		void Draw()
		{
			ImGui::Text("Hello, world %d", 123);
			if (ImGui::Button("CalculatePlots"))
				CalculatePlots();

			if (IsReady())
				DrawPlots();
		}

	private:
		std::vector<float> m_xValues{};
		std::vector<float> m_yValues{};
	};
}

#endif