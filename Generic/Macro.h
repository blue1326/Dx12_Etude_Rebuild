

#ifndef Macro_h__
#define Macro_h__

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif

#endif // Macro_h__

