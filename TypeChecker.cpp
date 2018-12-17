template<typename T, typename U>
class TypeChecker
{
	public:
		TypeChecker(T it1, U it2) : it1(it1), it2(it2) {};

		bool areBothInts(std::string toName, std::string fromName)
		{
			auto& toInts = it1->ints;
			auto& fromInts = it2->ints;

			return (toInts.find(toName) != toInts.end() && fromInts.find(fromName) != fromInts.end());
		}

		bool areBothIntVecs(std::string toName, std::string fromName)
		{
			auto& fromIntVecs = it1->intVecs;
			auto& toIntVecs = it2->intVecs;
			
			return (toIntVecs.find(toName) != toIntVecs.end() &&
			fromIntVecs.find(fromName) != fromIntVecs.end());
		}

	private:
		T it1;
		U it2;
};

