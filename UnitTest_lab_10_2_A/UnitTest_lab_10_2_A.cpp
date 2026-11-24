#include "pch.h"
#include "CppUnitTest.h"
#include "../lab_10_2_A/lab_10_2_A.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
namespace UnitTestlab102A
{
	TEST_CLASS(UnitTestlab102A)
	{
	public:
		
		TEST_METHOD(TestSumAndAverage)
		{
			Student p[2]{};

			p[0].prizv = "Коваленко";
			p[0].kurs = 2;
			p[0].speciality = Komp_Nauky;
			p[0].fizyka = 5;
			p[0].matematyka = 4;
			p[0].informatyka = 3; 

			p[1].prizv = "Бондар";
			p[1].kurs = 1;
			p[1].speciality = Informatyka;
			p[1].fizyka = 4;
			p[1].matematyka = 4;
			p[1].informatyka = 4; 

			Assert::AreEqual(12, Sum(p[0]));
			Assert::IsTrue(fabs(Average(p[0]) - 4.0) < 1e-6);

			Assert::AreEqual(12, Sum(p[1]));
			Assert::IsTrue(fabs(Average(p[1]) - 4.0) < 1e-6);
		}

		TEST_METHOD(TestSort)
		{
			Student p[2]{};

			p[0].prizv = "Іваненко";
			p[0].kurs = 3;
			p[0].speciality = Komp_Nauky;
			p[0].fizyka = 5;
			p[0].matematyka = 5;
			p[0].informatyka = 5; 

			p[1].prizv = "Петренко";
			p[1].kurs = 1;
			p[1].speciality = Trud;
			p[1].fizyka = 3;
			p[1].matematyka = 3;
			p[1].informatyka = 3; 

			Sort(p, 2);

			Assert::AreEqual(string("Петренко"), p[0].prizv);
			Assert::AreEqual(string("Іваненко"), p[1].prizv);
		}

		TEST_METHOD(TestIndexSort)
		{
			Student p[2]{};

			p[0].prizv = "Шевченко";
			p[0].kurs = 2;
			p[0].speciality = Komp_Nauky;
			p[0].fizyka = 5;
			p[0].matematyka = 5;
			p[0].informatyka = 5; 

			p[1].prizv = "Антоненко";
			p[1].kurs = 1;
			p[1].speciality = Trud;
			p[1].fizyka = 3;
			p[1].matematyka = 3;
			p[1].informatyka = 3; 

			int* idx = IndexSort(p, 2);

			Assert::AreEqual(1, idx[0]);
			Assert::AreEqual(0, idx[1]);

			delete[] idx;
		}

		TEST_METHOD(TestBinSearch)
		{
			Student p[2]{};

			p[0].prizv = "Петренко";
			p[0].kurs = 2;
			p[0].speciality = Trud;
			p[0].fizyka = 3;
			p[0].matematyka = 3;
			p[0].informatyka = 3; 

			p[1].prizv = "Іваненко";
			p[1].kurs = 3;
			p[1].speciality = Komp_Nauky;
			p[1].fizyka = 5;
			p[1].matematyka = 5;
			p[1].informatyka = 5; 

			Assert::AreEqual(1, BinSearch(p, 2, "Іваненко", 3, 5.0));
			Assert::AreEqual(-1, BinSearch(p, 2, "Невідомий", 1, 4.0));
		}
	};
}
