#ifndef _INT_H
#define _INT_H

/**
Code obtained from
https://github.com/z88dk/z88dk/tree/master/libsrc/_DEVELOPMENT/EXAMPLES/zx/demo_sp1/BlackStar

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef WFRAMES
// WFRAMES 3, are 50/3 -> 25 FPS.
#define WFRAMES  2
#endif

#define SPEED 6

#define clock(x) (tick)

extern unsigned char tick;
extern unsigned char pick;

void wait(void);
void setup_int(void);

void sonido2Sirena(void);
inline void sonido2Huida(void);
void start_ay(void);
void stop_ay(void);
inline void sonido1Bola(void);
inline void sonido3PierdeVida(void);
inline void sonido13ComeGhost(void);
inline void sonido1Fruta(void);
void stopCanal3(void);
void stopCanal2(void);
void incSiren(void);
void resetSiren(void);
inline void sonido3InsertCoin(void);

#endif
