#include "DominoDynamic.h"

namespace DominoDynamic {

	template <class T>
	void swap(T& a, T& b) {
		T p = a;
		a = b;
		b = p;
	}

	//Ã≈“Œƒ€ —“–” “”–€ ŒƒÕŒ… ƒŒÃ»ÕŒÿ »
	Domino::Domino(int bot, int top) : bot(bot), top(top) {
		if (bot > 7 || top > 7 || top < 0 || bot < 0)
			throw std::runtime_error("BAD VARIABLES!");
		if (bot > top)
			swap(this->bot, this->top);

	}

	Domino::~Domino()
	{
	}

	std::istream& operator>>(std::istream& stream, Domino& domino)
	{
		stream >> domino.bot >> domino.top;
		if (stream.good()) {
			if (domino.bot > 6 || domino.bot < 0 || domino.top > 6 || domino.top < 0)
				stream.setstate(std::ios::failbit);
			else {
				if (domino.bot > domino.top)
					swap(domino.bot, domino.top);
			}
		}
		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, Domino& domino)
	{
		stream << "(" << domino.bot << ", " << domino.top << ")" << std::endl;
		return stream;
	}

	bool operator==(Domino a, Domino b)
	{
		return (a.bot == b.bot && a.top == b.top || a.bot == b.top && a.top == b.bot);
	}

	bool operator>(Domino a, Domino b)
	{
		return (a.bot + a.top > b.top + b.bot);
	}

	bool operator==(int a, Domino b)
	{
		return (a == b.bot || a == b.top);
	}



	//Ã≈“Œƒ€  À¿——¿ ƒŒÃ»ÕŒ —≈“
	DominoSet::~DominoSet() {
		if (dominos != nullptr)
			delete[] dominos;
	}

	DominoSet::DominoSet(const DominoSet& set) : curSize(set.curSize)
	{
		dominos = new Domino[MAX_SIZE];
		for (int i = 0; i < curSize; i++) {
			dominos[i] = set.getDominos()[i];
		}
	}

	DominoSet::DominoSet(DominoSet&& set)
	{
		this->dominos = set.dominos;
		this->curSize = set.curSize;
		set.curSize = 0;
		//		set.dominos = nullptr;
	}

	DominoSet::DominoSet() : curSize(1)
	{
		Domino domino;
		dominos = new Domino[MAX_SIZE];
		dominos[0] = domino;
	}

	DominoSet::DominoSet(int count)
	{
		dominos = new Domino[MAX_SIZE];
		if (count > DominoSet::MAX_SIZE || count <= 0)
			throw std::runtime_error("BAD VARIABLE");
		for (int i = 0; i < count; i++) {
			addRandom();
		}
	}

	DominoSet::DominoSet(int bot, int top)
	{
		dominos = new Domino[MAX_SIZE];
		if (bot == top)
			throw std::runtime_error("BAD VARIABLES!");
		curSize = 1;
		dominos[0] = Domino(bot, top);
	}

	DominoSet::DominoSet(Domino domino)
	{
		dominos = new Domino[MAX_SIZE];
		dominos[0] = domino;
		curSize = 1;
	}

	void DominoSet::put(int index, Domino domino)
	{
		dominos[index] = domino;
	}

	bool DominoSet::contains(Domino domino) const
	{
		return find(domino) != -1;
	}

	void DominoSet::addRandom()
	{
		if (curSize == MAX_SIZE)
			throw std::runtime_error("OVERFLOW!");
		while (true) {
			Domino domino(rand() % 7, rand() % 7);
			if (contains(domino))
				continue;
			dominos[curSize] = domino;
			curSize++;
			break;
		}
	}

	void DominoSet::sort()
	{
		for (int i = 0; i < curSize; i++) {
			for (int j = i + 1; j < curSize; j++) {
				if (dominos[i] > dominos[j])
					swap(dominos[i], dominos[j]);
			}
		}
	}

	void DominoSet::remove(Domino domino)
	{
		int index = find(domino);
		if (index == -1)
			return;
		curSize--;
		dominos[index] = dominos[curSize];
	}

	void DominoSet::put(Domino domino)
	{
		if (curSize == MAX_SIZE)
			throw std::runtime_error("OVERFLOW!");
		if (contains(domino))
			return;
		dominos[curSize++] = domino;
	}

	DominoSet* DominoSet::getSubset(int value) const
	{
		DominoSet* newSet = new DominoSet;
		newSet->curSize = 0;

		if (value > 6 || value < 0)
			return newSet;

		for (int i = 0; i < curSize; i++) {
			if (value == dominos[i])
				(*newSet) += (dominos[i]);
		}
		return newSet;
	}

	DominoSet* DominoSet::openByNumber(int number) const
	{
		if (number > curSize)
			throw std::runtime_error("BAD VARIABLE!");
		DominoSet* set = new DominoSet;
		set->put(0, dominos[number]);
		set->curSize = 0;
		return set;
	}

	DominoSet& DominoSet::operator=(const DominoSet& a)
	{
		delete[] this->dominos;
		this->dominos = new Domino[MAX_SIZE];
		curSize = a.curSize;
		for (int i = 0; i < curSize; i++)
			put(i, a.dominos[i]);
		return *this;
	}

	DominoSet& DominoSet::operator=(DominoSet&& set)
	{
		return *(new DominoSet(set));
	}

	Domino& DominoSet::operator[](int a)
	{
		return dominos[a];
	}

	void operator++(DominoSet& set)
	{
		set.addRandom();
	}

	void operator--(DominoSet& set)
	{
		set.remove(set[(set.getCurSize() - 1)]);
	}

	DominoSet& operator+=(DominoSet& set, Domino domino)
	{
		set.put(domino);
		return set;
	}

	DominoSet* operator+(DominoSet& set0, DominoSet& set1)
	{
		DominoSet* set = new DominoSet();
		Domino domino;
		set->remove(domino);
		for (int i = 0; i < set0.getCurSize(); i++)
			*set += set0.getDominos()[i];
		for (int i = 0; i < set1.getCurSize(); i++)
			*set += set1.getDominos()[i];
		return set;
	}

	DominoSet& operator-=(DominoSet& set, Domino domino)
	{
		set.remove(domino);
		return set;
	}

	int DominoSet::getCurSize() const
	{
		return this->curSize;
	}

	int DominoSet::find(Domino domino) const
	{
		for (int i = 0; i < curSize; i++)
			if (domino == dominos[i])
				return i;
		return -1;
	}

	Domino* DominoSet::getDominos() const
	{
		return dominos;
	}


}