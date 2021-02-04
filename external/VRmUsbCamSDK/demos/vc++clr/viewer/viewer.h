#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

public ref class Viewer : public System::Windows::Forms::Form
{
public:
	Viewer(Drawing::Size size)
	{
		InitializeComponent();

		ClientSize = size;
	}

protected:
	~Viewer()
	{
		if (components)
			delete components;
	}

	virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs ^ e) override 
	{
		if (e->KeyCode == Keys::Escape) Hide();
	}

private:
	System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Viewer::typeid));
		this->SuspendLayout();
		// 
		// Viewer
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(292, 266);
		this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
		this->Name = L"Viewer";
		this->Text = L"Viewer";
		this->ResumeLayout(false);

	}
#pragma endregion
};
