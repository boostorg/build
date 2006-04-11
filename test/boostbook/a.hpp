
// Seems like Boostbook does like classes outside of namespaces,
// and won't generate anything for them.
namespace boost {

/// A class
class A {
public:
    /// A constructor
    A();    
};
}