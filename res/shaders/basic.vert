#version 330 core // powiedzenie z jakiej wersji opengla korzystamy. najni�sza targetowana wersja
layout(location = 0) in vec3 aPos; // [layout(location = 0)]- zaczynaj�c od pocz�tku, bez przesuni�cia w buforze
                                   // [in]-wczytaj [vec3]-trzy floaty [aPos]-do zmiennej o nazwie aPos i typie Vec3

uniform mat4 model; // Zmienna kt�r� mo�na przekaza� do shadera z CPU

void main()
{
    // gl_Position - zmienna do kt�rej MUSISZ wpisa� pozycje vertexa. Za pomoc� tej zmiennej s� przekazywane dane do fragment shadera (rysunek)
    gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0); // Jak jest 1.0 to mamy pozycj� (punkt), jak 0.0 to mamy wektor
}
// koniec