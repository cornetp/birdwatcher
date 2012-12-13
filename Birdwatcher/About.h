#pragma once

#include <stdio.h>
#include "resource.h"

namespace Birdwatcher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for About
	/// </summary>
	public ref class About : public System::Windows::Forms::Form
	{
	public:
		About(void)
		{
			InitializeComponent();
			char str[64];
			sprintf_s(str, "BirdWatcher v.%d.%d.0.%d%s", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_APPENDIX);
			this->bwVersionLabel->Text = gcnew String(str);

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~About()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::LinkLabel^  authorWebsiteLabel;
	protected: 

	private: System::Windows::Forms::Label^  bwVersionLabel;
	private: System::Windows::Forms::Label^  authorLabel;


	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->authorWebsiteLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->bwVersionLabel = (gcnew System::Windows::Forms::Label());
			this->authorLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// authorWebsiteLabel
			// 
			this->authorWebsiteLabel->Cursor = System::Windows::Forms::Cursors::Hand;
			this->authorWebsiteLabel->Location = System::Drawing::Point(12, 71);
			this->authorWebsiteLabel->Name = L"authorWebsiteLabel";
			this->authorWebsiteLabel->Size = System::Drawing::Size(203, 18);
			this->authorWebsiteLabel->TabIndex = 0;
			this->authorWebsiteLabel->TabStop = true;
			this->authorWebsiteLabel->Text = L"Author\'s Website";
			this->authorWebsiteLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->authorWebsiteLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &About::linkLabel1_LinkClicked);
			// 
			// bwVersionLabel
			// 
			this->bwVersionLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->bwVersionLabel->Location = System::Drawing::Point(12, 21);
			this->bwVersionLabel->Name = L"bwVersionLabel";
			this->bwVersionLabel->Size = System::Drawing::Size(203, 15);
			this->bwVersionLabel->TabIndex = 1;
			this->bwVersionLabel->Text = L"BirdWatcher v 1.0.0.0";
			this->bwVersionLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// authorLabel
			// 
			this->authorLabel->Location = System::Drawing::Point(12, 46);
			this->authorLabel->Name = L"authorLabel";
			this->authorLabel->Size = System::Drawing::Size(203, 17);
			this->authorLabel->TabIndex = 2;
			this->authorLabel->Text = L"© Philippe Cornet 2011";
			this->authorLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// About
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(227, 111);
			this->Controls->Add(this->authorLabel);
			this->Controls->Add(this->bwVersionLabel);
			this->Controls->Add(this->authorWebsiteLabel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"About";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"About";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void linkLabel1_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
				 authorWebsiteLabel->LinkVisited = true;
				 System::Diagnostics::Process::Start("http://www.philippecornet.be");     
			 }
	};
}
