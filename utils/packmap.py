# Empaqueta el array "mapa" de assets/mapa.h a 4 bits por tile (2 tiles por byte).
# Uso: python packmap.py assets\mapa.h     (ejecutar DESPUES de mapcnv.exe)
# - Genera map_tab[16] (indice -> tile real) y mapa[] empaquetado.
# - Primer tile de cada pareja = nibble alto, segundo = nibble bajo.
# - Lo que mapcnv escribe despues del array (cerrojos...) se conserva.
import re, sys
p = sys.argv[1]
s = open(p, encoding='latin-1', newline='').read()
if 'map_tab' in s:
    sys.exit('El mapa ya esta empaquetado.')
a = s.index('unsigned char mapa'); b = s.index('};', a) + 2
tiles = [int(x) for x in re.findall(r'\d+', s[s.index('{', a):b])]
n = len(tiles)
if n % 2: tiles.append(tiles[-1])
tab = sorted(set(tiles))
if len(tab) > 16:
    sys.exit('ERROR: %d tiles distintos, el maximo es 16: %s' % (len(tab), tab))
idx = {t: i for i, t in enumerate(tab)}
packed = [(idx[tiles[i]] << 4) | idx[tiles[i + 1]] for i in range(0, len(tiles), 2)]
tab16 = tab + [0] * (16 - len(tab))
nl = '\r\n' if '\r\n' in s else '\n'
out = 'unsigned char map_tab [16] = { %s };%s' % (', '.join(map(str, tab16)), nl)
out += 'unsigned char mapa [] = {' + nl
for i in range(0, len(packed), 15):
    out += '    ' + ', '.join(map(str, packed[i:i + 15])) + ',' + nl
out = out.rstrip(',' + nl) + nl + '};'
open(p, 'w', encoding='latin-1', newline='').write(s[:a] + out + s[b:])
print('mapa: %d tiles -> %d bytes (tabla: %s)' % (n, len(packed), tab))
