#version 330 core // powiedzenie z jakiej wersji opengla korzystamy. najni¿sza targetowana wersja
layout(location = 0) in vec3 aPos; // [layout(location = 0)]- zaczynaj¹c od pocz¹tku, bez przesuniêcia w buforze
                                   // [in]-wczytaj [vec3]-trzy floaty [aPos]-do zmiennej o nazwie aPos i typie Vec3
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoord;

out vec2 Texcoord;

uniform mat4 model; // Zmienna któr¹ mo¿na przekazaæ do shadera z CPU

void main()
{
    Texcoord = aTexcoord;
    // gl_Position - zmienna do której MUSISZ wpisaæ pozycje vertexa. Za pomoc¹ tej zmiennej s¹ przekazywane dane do fragment shadera (rysunek)
    gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0); // Jak jest 1.0 to mamy pozycjê (punkt), jak 0.0 to mamy wektor
}
// koniec