#pragma once
#include <list>
#include <msclr\marshal_cppstd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;


	public ref class MyForm : public System::Windows::Forms::Form
	{
		cli::array<System::Windows::Forms::Label^, 2>^ texts = gcnew cli::array<Label^, 2>(12, 12); //cream un vector de obiecte Lable pentru afisarea numarului sub bari
	public:
		cli::array<int, 2>^ b = gcnew cli::array<int, 2>(12, 12);
		cli::array<int, 2>^ coordonate = gcnew cli::array<int, 2>(4, 1000);

		cli::array<int, 4>^ drumuri = gcnew cli::array<int, 4>(12,12,122,3);

		PictureBox^ fundal = gcnew PictureBox();

		int limita;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;

	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button6;
	private: System::Windows::Forms::Button^ button7;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;



	public:
	public:

	public:


	public:
		System::ComponentModel::Container^ components;

		MyForm(void)
		{
			InitializeComponent();
		}

		//fuctie responsabila de desenarea patratelelor 
		void desenare()
		{
			char aux[3];
			//culorile patratelelor
			int C = 230, c;
			//dimensiuni si locatii
			int dim_x = 40, dim_y = 40, spatiu_sus=50, spatiu_lateral=100, spatiu_intre = 3;

			Controls->Add(fundal);
			fundal->Size = Drawing::Size(11 * (dim_x + spatiu_intre) + spatiu_intre, 11 * (dim_y + spatiu_intre) + spatiu_intre);
			fundal->Location = Point(spatiu_lateral + dim_x, spatiu_sus+dim_y);
			fundal->BackColor = Color::SkyBlue;

			for(int i=1;i<=11;i++)
				for (int j = 1; j <= 11; j++)
				{
					//cream obiectele
					texts[i, j] = gcnew Label();

					//adaugam patratele cu text
					this->Controls->Add(texts[i, j]);

					//setam dimensiunile si locatile in form
					texts[i, j]->Location = Point(j * (dim_x + spatiu_intre) + spatiu_lateral, i * (dim_y + spatiu_intre) + spatiu_sus);
					texts[i, j]->Size = Drawing::Size(dim_x, dim_y);

					
					//setam culoarea
					//daca patratelul este teleport il coloram in verde, iar valoare afisata va fi 0
					if (b[i, j] == -1)
					{
						texts[i, j]->BackColor = Color::Green;
						texts[i, j]->Text = "0";
					}
					else
					{
						itoa(b[i,j],aux,10);
						c = C - 40 * b[i, j];
						texts[i, j]->BackColor = Color::FromArgb(static_cast<Int32>(static_cast<Byte>(c)), static_cast<Int32>(static_cast<Byte>(c)), static_cast<Int32>(static_cast<Byte>(c)));
						texts[i, j]->Text = msclr::interop::marshal_as<String^>(aux);
					}
					texts[i, j]->BringToFront();
					texts[i, j]->TextAlign = ContentAlignment::MiddleCenter;
					texts[i, j]->ForeColor = Color::Black;
					texts[i, j]->Font = (gcnew Drawing::Font(L"Times New Roman", 12.0F, Drawing::FontStyle::Bold, Drawing::GraphicsUnit::Point,static_cast<Byte>(0)));
				}
			
		}
		void miscari()
		{
			Color cul;
			char aux[3];
			for (int i = 1; i <= limita; i++)
			{
				if (i > 1)
				{
					texts[coordonate[1, i - 1], coordonate[2, i - 1]]->BackColor = cul;
					texts[coordonate[1, i - 1], coordonate[2, i - 1]]->Refresh();

				}
				cout << coordonate[1, i] << ", " << coordonate[2, i] << endl;
				cul = texts[coordonate[1, i], coordonate[2, i]]->BackColor;
				if (coordonate[3, i] != INT_MAX)
				{
					itoa(coordonate[3, i], aux, 10);
					texts[coordonate[1, i], coordonate[2, i]]->Text = msclr::interop::marshal_as<String^>(aux);
					texts[coordonate[1, i], coordonate[2, i]]->ForeColor = Color::FromArgb(static_cast<Int32>(static_cast<Byte>(255)), static_cast<Int32>(static_cast<Byte>(60)), static_cast<Int32>(static_cast<Byte>(60)));
					
				}
				texts[coordonate[1, i], coordonate[2, i]]->BackColor = Color::FromArgb(static_cast<Int32>(static_cast<Byte>(255)), static_cast<Int32>(static_cast<Byte>(128)), static_cast<Int32>(static_cast<Byte>(128)));
				texts[coordonate[1, i], coordonate[2, i]]->Refresh();

				Sleep(100);
			}
			texts[coordonate[1, limita], coordonate[2, limita]]->BackColor = cul;
			texts[coordonate[1, limita], coordonate[2, limita]]->Refresh();
			label3->Text = texts[11, 11]->Text;
			label3->BackColor = pictureBox1->BackColor;
			label3->Font = (gcnew Drawing::Font(L"Times New Roman", 20, Drawing::FontStyle::Bold, Drawing::GraphicsUnit::Point, static_cast<Byte>(0)));
		}
		void desenare_drum(int x, int y)
		{
			for (int i = 1; i <= drumuri[x, y, 0, 0]; i++)
			{
				cout << drumuri[x, y, i, 1] << " " << drumuri[x, y, i, 2] << endl;
				texts[drumuri[x, y, i, 1], drumuri[x, y, i, 2]]->BackColor = Color::Aquamarine;
			}
			texts[x, y]->BackColor = Color::Aquamarine;
		}
		void reparare_drum(int x, int y)
		{
			if(x==-1)
			{
				x = 1; y = 1;
			}
			char aux[3];
			int c, C = 230;
			for (int i = 1; i <= drumuri[x, y, 0, 0]; i++)
			{
				if (b[drumuri[x, y, i, 1], drumuri[x, y, i, 2]] == -1)
					texts[drumuri[x, y, i, 1], drumuri[x, y, i, 2]]->BackColor = Color::Green;
				else
				{

					itoa(b[drumuri[x, y, i, 1], drumuri[x, y, i, 2]], aux, 10);
					c = C - 40 * b[drumuri[x, y, i, 1], drumuri[x, y, i, 2]];
					texts[drumuri[x, y, i, 1], drumuri[x, y, i, 2]]->BackColor = Color::FromArgb(static_cast<Int32>(static_cast<Byte>(c)), static_cast<Int32>(static_cast<Byte>(c)), static_cast<Int32>(static_cast<Byte>(c)));

				}
			}
			if (b[x,y] == -1)
				texts[x,y]->BackColor = Color::Green;
			else
			{

				itoa(b[x,y], aux, 10);
				c = C - 40 * b[x,y];
				texts[x,y]->BackColor = Color::FromArgb(static_cast<Int32>(static_cast<Byte>(c)), static_cast<Int32>(static_cast<Byte>(c)), static_cast<Int32>(static_cast<Byte>(c)));

			}
		}
	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button1->Location = System::Drawing::Point(821, 184);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(127, 49);
			this->button1->TabIndex = 0;
			this->button1->Text = L"calculeaza distanta pana la finish";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button2->Location = System::Drawing::Point(821, 259);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(127, 49);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Evidentiaza drumul cel mai optim";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button4->Location = System::Drawing::Point(922, 532);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 3;
			this->button4->Text = L"dreapta";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button5->Location = System::Drawing::Point(772, 532);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 4;
			this->button5->Text = L"stanga";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// button6
			// 
			this->button6->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button6->Location = System::Drawing::Point(848, 557);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(75, 23);
			this->button6->TabIndex = 5;
			this->button6->Text = L"jos";
			this->button6->UseVisualStyleBackColor = false;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// button7
			// 
			this->button7->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button7->Location = System::Drawing::Point(848, 506);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(75, 23);
			this->button7->TabIndex = 6;
			this->button7->Text = L"sus";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &MyForm::button7_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->pictureBox1->Location = System::Drawing::Point(751, 169);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(261, 437);
			this->pictureBox1->TabIndex = 7;
			this->pictureBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(769, 457);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(228, 13);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Muta casuta pentru care se evidentiaza drumul";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(804, 338);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(161, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Cel mai optim drum are lungimea:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->label3->Location = System::Drawing::Point(860, 377);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(0, 13);
			this->label3->TabIndex = 10;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1111, 655);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		bool drumul_este_calculat = false;
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		reparare_drum(X,Y);
		miscari();
		drumul_este_calculat = true;
	}
		   int X=-1, Y;
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	if (drumul_este_calculat) {
		X = 11;
		Y = 11;
		desenare_drum(X, Y);
	}
}

private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) {
	if (X > 1)
	{
		reparare_drum(X, Y);
		X--;
		desenare_drum(X, Y);
	}
}
private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
	if (X <11 && X!=-1)
	{
		reparare_drum(X, Y);
		X++;
		desenare_drum(X, Y);
	}
}
private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {
	if (Y > 1 && X != -1)
	{
		reparare_drum(X, Y);
		Y--;
		desenare_drum(X, Y);
	}
}
private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
	if (Y < 11 && X != -1)
	{
		reparare_drum(X, Y);
		Y++;
		desenare_drum(X, Y);
	}

}
};
}
