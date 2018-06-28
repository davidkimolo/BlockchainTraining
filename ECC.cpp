#include <cstdlib>
 33  #include <iostream>
 34  #include <vector>
 35  using namespace std;
 36  
 37  #include <math.h>
 38  #include "FiniteFieldElement.hpp"
 39  
 40  namespace Cryptography
 41  {    
 53          
 54          template<int P>
 55          class   EllipticCurve
 56          {
 57              public:

 60                  typedef FiniteFieldElement<P> ffe_t;

 67                  class   Point
 68                  {
 69                      friend  class   EllipticCurve<P>;
 70                      typedef FiniteFieldElement<P> ffe_t;
 71                      ffe_t  x_;
 72                      ffe_t  y_;
 73                      EllipticCurve    *ec_;

 77                      void   addDouble(int m, Point& acc)
 78                      {        
 79                          if ( m > 0 )
 80                          {       
 81                              Point r = acc; 
 82                              for ( int n=0; n < m; ++n )
 83                              {
 84                                  r += r;     // doubling step                          
 85                              }
 86                              acc = r;
 87                          }        
 88                      }

 93                      Point scalarMultiply(int k, const Point& a)
 94                      {
 95                          Point acc = a;
 96                          Point res = Point(0,0,*ec_);
 97                          int i = 0, j = 0;
 98                          int b = k;
 99                          
100                          while( b )
101                          {
102                              if ( b & 1 )
103                              {
105                                  addDouble(i-j,acc);
106                                  res += acc;           
107                                  j = i;  
108                              }
109                              b >>= 1;
110                              ++i;
111                          }
112                          return res;
113                      }
115                      void    add(ffe_t x1, ffe_t y1, ffe_t x2, ffe_t y2, ffe_t & xR, ffe_t & yR) const
116                      {
118                          if ( x1 == 0 && y1 == 0 ) 
119                          {
120                              xR = x2;
121                              yR = y2;
122                              return;
123                          }
124                          if ( x2 == 0 && y2 == 0 )
125                          {
126                              xR = x1;
127                              yR = y1;
128                              return;
129                          }
130                          if ( y1 == -y2 ) 
131                          {
132                              xR = yR = 0;
133                              return;
134                          }
135                                                  
137                          ffe_t s;                                                
138                          if ( x1 == x2 && y1 == y2 )
139                          {
141                              s = (3*(x1.i()*x1.i()) + ec_->a()) / (2*y1);
142                              xR = ((s*s) - 2*x1);                            
143                          }
144                          else
145                          {   
147                              s = (y1 - y2) / (x1 - x2);
148                              xR = ((s*s) - x1 - x2);
149                          }
150                          
151                          if ( s != 0 )
152                          {
153                              yR = (-y1 + s*(x1 - xR));
154                          }
155                          else
156                          {
157                              xR = yR = 0;
158                          }           
159                      }
160                      
161                      Point(int x, int y)
162                      : x_(x),
163                        y_(y),
164                        ec_(0)
165                      {}
166                                    
167                      Point(int x, int y, EllipticCurve<P> & EllipticCurve)
168                       : x_(x),
169                         y_(y),
170                         ec_(&EllipticCurve)
171                      {}
172                      
173                      Point(const ffe_t& x, const ffe_t& y, EllipticCurve<P> & EllipticCurve)
174                       : x_(x),
175                         y_(y),
176                         ec_(&EllipticCurve)
177                      {}
178                      
179                  public:                    
180                      static  Point   ONE;
181                      
183                      Point(const Point& rhs)
184                      {
185                          x_ = rhs.x_;
186                          y_ = rhs.y_;
187                          ec_ = rhs.ec_;
188                      }
190                      Point& operator=(const Point& rhs)
191                      {
192                          x_ = rhs.x_;
193                          y_ = rhs.y_;
194                          ec_ = rhs.ec_;
195                          return *this;
196                      }
198                      ffe_t x() const { return x_; }
200                      ffe_t y() const { return y_; }
201  
204                      unsigned int     Order(unsigned int maxPeriod = ~0) const
205                      {
206                          Point r = *this;
207                          unsigned int n = 0;
208                          while( r.x_ != 0 && r.y_ != 0 )
209                          {
210                              ++n;
211                              r += *this;
212                              if ( n > maxPeriod ) break;
213                          }
214                          return n;
215                      }
217                      Point   operator-()
218                      {
219                          return Point(x_,-y_);
220                      }                                        
222                      friend bool    operator==(const Point& lhs, const Point& rhs)
223                      {
224                          return (lhs.ec_ == rhs.ec_) && (lhs.x_ == rhs.x_) && (lhs.y_ == rhs.y_);
225                      }
227                      friend bool    operator!=(const Point& lhs, const Point& rhs)
228                      {
229                          return (lhs.ec_ != rhs.ec_) || (lhs.x_ != rhs.x_) || (lhs.y_ != rhs.y_);
230                      }                    
232                      friend Point operator+(const Point& lhs, const Point& rhs)
233                      {       
234                          ffe_t xR, yR;
235                          lhs.add(lhs.x_,lhs.y_,rhs.x_,rhs.y_,xR,yR);
236                          return Point(xR,yR,*lhs.ec_);    
237                      }
239                      friend  Point operator*(int k, const Point& rhs)
240                      {
241                          return Point(rhs).operator*=(k);
242                      }
244                      Point& operator+=(const Point& rhs)
245                      {   
246                          add(x_,y_,rhs.x_,rhs.y_,x_,y_);
247                          return *this;  
248                      }
250                      Point& operator*=(int k)
251                      {
252                          return (*this = scalarMultiply(k,*this));
253                      }                    
255                      friend ostream& operator <<(ostream& os, const Point& p)
256                      {
257                          return (os << "(" << p.x_ << ", " << p.y_ << ")");
258                      }
259                  };
260                  
262                  
263                  typedef EllipticCurve<P> this_t;
264                  typedef class EllipticCurve<P>::Point point_t;
265                  
268                  EllipticCurve(int a, int b)
269                  : a_(a),
270                    b_(b),
271                    m_table_(),
272                    table_filled_(false)
273                  {                    
274                  }

277                  void    CalculatePoints()
278                  {
279                      int x_val[P];
280                      int y_val[P];
281                      for ( int n = 0; n < P; ++n )
282                      {
283                          int nsq = n*n;
284                          x_val[n] = ((n*nsq) + a_.i() * n + b_.i()) % P;
285                          y_val[n] = nsq % P;                        
286                      }
287                      
288                      for ( int n = 0; n < P; ++n )
289                      {
290                          for ( int m = 0; m < P; ++m )
291                          {
292                              if ( x_val[n] == y_val[m] )
293                              {
294                                  m_table_.push_back(Point(n,m,*this));
295                              }
296                          }
297                      }
298                      
299                      table_filled_ = true;
300                  }
302                  Point   operator[](int n)
303                  {
304                      if ( !table_filled_ )
305                      {
306                          CalculatePoints();
307                      }
308                      
309                      return m_table_[n];
310                  }
312                  size_t  Size() const { return m_table_.size(); }
314                  int     Degree() const { return P; }
316                  FiniteFieldElement<P>  a() const { return a_; }
318                  FiniteFieldElement<P>  b() const { return b_; }
319                  
321                  template<int T>
322                  friend ostream& operator <<(ostream& os, const EllipticCurve<T>& EllipticCurve);                       
324                  ostream&    PrintTable(ostream &os, int columns=4);
325                  
326                  private:
327                      typedef std::vector<Point>  m_table_t;
328                      
329                      m_table_t                   m_table_;   // table of points
330                      FiniteFieldElement<P>       a_;         // paramter a of the EC equation
331                      FiniteFieldElement<P>       b_;         // parameter b of the EC equation
332                      bool    table_filled_;                  // true if the table has been calculated
333          };
334          
335          template<int T>
336              typename EllipticCurve<T>::Point EllipticCurve<T>::Point::ONE(0,0);
337                                 
338          template<int T>
339          ostream& operator <<(ostream& os, const EllipticCurve<T>& EllipticCurve)
340          {
341              os << "y^2 mod " << T << " = (x^3" << showpos;
342              if ( EllipticCurve.a_ != 0 )
343              {
344                  os << EllipticCurve.a_ << "x";                
345              }
346              
347              if ( EllipticCurve.b_.i() != 0 )
348              {
349                  os << EllipticCurve.b_; 
350              }
351              
352              os << noshowpos << ") mod " << T;
353              return os;
354          }
355          
356          template<int P>
357          ostream&    EllipticCurve<P>::PrintTable(ostream &os, int columns)
358          {
359              if ( table_filled_ )
360              {
361                  int col = 0;
362                  typename EllipticCurve<P>::m_table_t::iterator iter = m_table_.begin();
363                  for ( ; iter!=m_table_.end(); ++iter )
364                  {
365                      os << "(" << (*iter).x_.i() << ", " << (*iter).y_.i() << ") ";
366                      if ( ++col > columns )
367                      {
368                          os << "\n";
369                          col = 0;
370                      }
371                  }
372              }
373              else
374              {
375                  os << "EllipticCurve, F_" << P;
376              }
377              return os;
378          }                        
379  }
380  
381  namespace   utils
382  {    
383      float   frand()
384      {
385          static float norm = 1.0f / (float)RAND_MAX;
386          return (float)rand()*norm;
387      }
388      
389      int irand(int min, int max)
390      {
391          return min+(int)(frand()*(float)(max-min));
392      }
393  }
394  
395  using namespace Cryptography;
396  using namespace utils;
397  
398  int main(int argc, char *argv[])
399  {
400      typedef EllipticCurve<263> ec_t;
401      ec_t   myEllipticCurve(1,1);
402      
403      cout << "A little Elliptic Curve cryptography example\n\n\n";
404      
406      cout << "The elliptic curve: " << myEllipticCurve << "\n";
407      
410      myEllipticCurve.CalculatePoints();
411      
412      cout << "\nPoints on the curve (i.e. the group elements):\n";
413      myEllipticCurve.PrintTable(cout,5);
414      cout << "\n\n";
415      
416      ec_t::Point P = myEllipticCurve[2];
417      cout << "some point P  = " << P << ", 2P = " << (P+P) << "\n";    
418      ec_t::Point Q = myEllipticCurve[3];
419      cout << "some point Q = " << Q << ", P+Q = " << (P+Q) << "\n"; 
420      ec_t::Point R = P;
421      R += Q;
422      cout << "P += Q = " << R << "\n";
423      R = P;
424      R += R;
425      cout << "P += P = 2P = " << R << "\n";
426      
427      cout << "\nEC message encryption example\n===============================================\n\n";
428      

434      while( (G.y() == 0 || G.x() == 0) || (G.Order()<2) )
435      {
436          int n = (int)(frand()*myEllipticCurve.Size());
437          G = myEllipticCurve[n];
438      }
439      
440      cout << "G = " << G << ", order(G) is " << G.Order() << "\n";
441      
443      int a = irand(1,myEllipticCurve.Degree()-1);
444      ec_t::Point Pa = a*G;  // public key
445      cout << "Alice' public key Pa = " << a << "*" << G << " = " << Pa << endl;    
446          
448      int b = irand(1,myEllipticCurve.Degree()-1);;
449      ec_t::Point Pb = b*G;  // public key       
450      cout << "Bob's public key Pb = " << b << "*" << G << " = " << Pb << endl;    
451      
453      int j = irand(1,myEllipticCurve.Degree()-1);;
454      ec_t::Point Pj = j*G;
455      cout << "Jane's public key Pj = " << j << "*" << G << " = " << Pj << endl;    
456  
457      cout << "\n\n";
458      
464      int m1 = 19;
465      int m2 = 72;
466      
467      cout << "Plain text message from Alice to Bob: (" << m1 << ", " << m2 << ")\n";
468   
470      ec_t::Point Pk = a*Pb;
471      ec_t::ffe_t c1( m1*Pk.x() );
472      ec_t::ffe_t c2( m2*Pk.y() );
473      
475      cout << "Encrypted message from Alice to Bob = {Pa,c1,c2} = {" << Pa << ", " << c1 << ", " << c2 << "}\n\n";
476      
478      Pk = b*Pa;
479      ec_t::ffe_t m1d = c1/Pk.x();
480      ec_t::ffe_t m2d = c2/Pk.y();
481      
482      cout << "\tBob's decrypted message from Alice = (" << m1d << ", " << m2d << ")" << endl;
483      
485      Pk = j*Pa;
486      m1d = c1/Pk.x();
487      m2d = c2/Pk.y();
488  
489      cout << "\nJane's decrypted message from Alice = (" << m1d << ", " << m2d << ")" << endl;
490      
491      cout << endl;
492      
493      system("PAUSE");
494      return EXIT_SUCCESS;
495  }
