#pragma once

class WorldEditor : public Nt::Window {
public:
	enum {
		FIELD_ADDITIONHEIGHT,
		FIELD_FREQUENCY,
		FIELD_AMPLITUDE,
		FIELD_LACUNARITY,
		FIELD_GAIN,
		FIELD_NUMOCTAVESNOISE,
		FIELD_COUNT
	};
	enum {
		BUTTON_APPLY_TERRAIN,
		BUTTON_RECOMPILE_SHADER,
	};

public:
	WorldEditor() noexcept = default;

	void Initialize(Nt::Shader* pShader, Terrain* pTerrain, const Terrain::Config& config) {
		m_pShader = RequireNotNull(pShader);
		m_pTerrain = RequireNotNull(pTerrain);
		m_Config = config;

		Create(Nt::IntRect(100, 100, 400, 400), "Terrain Editor");

		_InitializeControls();

		Show();
	}

private:
	std::vector<Nt::TextEdit> m_TextEdits;
	std::vector<Nt::Text> m_Texts;

	Nt::Button m_ApplyTerrainButton;
	Nt::Button m_RecompileShaderButton;

	Nt::Shader* m_pShader;
	Terrain* m_pTerrain;
	Terrain::Config m_Config;

private:
	void _InitializeControls() {
		constexpr std::string_view labels[FIELD_COUNT] = {
			"Addition Height",
			"Frequency",
			"Amplitude",
			"Lacunarity",
			"Gain",
			"Num Octaves",
		};

		Int rectBottom = 5;
		for (uInt i = 0; i < FIELD_COUNT; ++i) {
			Nt::Text text;
			text.AddFormat(DT_SINGLELINE);
			text.SetPosition(Nt::Int2D(5, rectBottom));
			text.SetText(labels[i]);
			text.SetColor({ 0, 0, 0 });
			text.SetWeight(10);

			m_Texts.push_back(std::move(text));

			rectBottom += 30;
		}

		rectBottom = 5;
		for (uInt i = 0; i < FIELD_COUNT; ++i) {
			Nt::TextEdit textEdit;
			textEdit.SetID(i);
			textEdit.SetParent(*this);
			textEdit.Create({ 95, rectBottom, 300, 20 }, "0.0", true);
			textEdit.Show();

			m_TextEdits.push_back(std::move(textEdit));

			rectBottom += 30;
		}

		Terrain::Config::Property editedProperty = m_Config.Properties[0][0];
		m_TextEdits[FIELD_ADDITIONHEIGHT].SetText(editedProperty.AdditionHeight);
		m_TextEdits[FIELD_FREQUENCY].SetText(editedProperty.Frequency);
		m_TextEdits[FIELD_AMPLITUDE].SetText(editedProperty.Amplitude);
		m_TextEdits[FIELD_LACUNARITY].SetText(editedProperty.Lacunarity);
		m_TextEdits[FIELD_GAIN].SetText(editedProperty.Gain);
		m_TextEdits[FIELD_NUMOCTAVESNOISE].SetText(editedProperty.NumOctaves);

		rectBottom += 5;
		m_ApplyTerrainButton.SetID(BUTTON_APPLY_TERRAIN);
		m_ApplyTerrainButton.SetParent(*this);
		m_ApplyTerrainButton.Create({ 5, rectBottom, 340, 30 }, "Apply terrain");
		m_ApplyTerrainButton.Show();

		rectBottom += m_ApplyTerrainButton.GetWindowRect().Bottom + 5;
		m_RecompileShaderButton.SetID(BUTTON_RECOMPILE_SHADER);
		m_RecompileShaderButton.SetParent(*this);
		m_RecompileShaderButton.Create({ 5, rectBottom, 340, 30 }, "Recompile shader");
		m_RecompileShaderButton.Show();
	}

	void _RecompileShader() {
		try {
			m_pShader->Delete();
			m_pShader->Initialize();
			m_pShader->CompileFromFile(Nt::Shader::Types::VERTEX, "Shaders/Vert.glsl");
			m_pShader->CompileFromFile(Nt::Shader::Types::FRAGMENT, "Shaders/Frag.glsl");
			m_pShader->Link();
			m_pShader->Use();
		}
		catch (const Nt::Error& error) {
			error.Show();
		}
	}

	void _Command(const Long& param_1, const Long& param_2) final override {
		const uInt command = HIWORD(param_1);
		const uInt elementID = LOWORD(param_1);

		Terrain::Config::Property editedProperty = m_Config.Properties[0][0];

		switch (command) {
		case BN_CLICKED:
			switch (elementID) {
			case BUTTON_APPLY_TERRAIN:
				m_pTerrain->FractalBrownianMotionNoise(m_Config);
				break;

			case BUTTON_RECOMPILE_SHADER:
				_RecompileShader();
				break;
			}
			break;

		case EN_UPDATE:
		{
			Nt::String value = m_TextEdits[elementID].GetText();

			try {
				switch (elementID) {
				case FIELD_ADDITIONHEIGHT:
					editedProperty.AdditionHeight = value.ToFloat();
					break;

				case FIELD_FREQUENCY:
					editedProperty.Frequency = value.ToFloat();
					break;

				case FIELD_AMPLITUDE:
					editedProperty.Amplitude = value.ToFloat();
					break;

				case FIELD_LACUNARITY:
					editedProperty.Lacunarity = value.ToFloat();
					break;

				case FIELD_GAIN:
					editedProperty.Gain = value.ToFloat();
					break;

				case FIELD_NUMOCTAVESNOISE:
					editedProperty.Frequency = value;
					break;

				}
			}
			catch (...) 
			{
			}
		}
			break;
		}

		for (std::vector<Terrain::Config::Property>& properties : m_Config.Properties) {
			for (Terrain::Config::Property& property : properties)
				property = editedProperty;
		}
	}

	void _Paint([[maybe_unused]] HDC& hdc, [[maybe_unused]] PAINTSTRUCT& paint) override {
		for (Nt::Text& text : m_Texts)
			text.Draw(hdc);
	}
};