//source: Simplify C++ Arne Mertz blog
//https://arne-mertz.de/2018/05/overload-build-a-variant-visitor-on-the-fly/
template <class ...Fs>
struct LambdaVisitor : Fs... {
	  LambdaVisitor(Fs const&... fs) : Fs{fs}...
		    {}

	    using Fs::operator()...;
};
