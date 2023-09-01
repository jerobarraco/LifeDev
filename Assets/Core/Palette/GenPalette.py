#!/usr/bin/env python3


def main():
	o = open("/home/nande/work/repos/LifeDev/Assets/Core/Palette/EditorPerProjectUserSettings.gen.ini", "w")
	f = open("/home/nande/work/repos/LifeDev/Assets/Core/Palette/atari-8-bit-series-gtia.hex", "r")
	theme = 0
	m = 16
	count = 0
	for l in f.readlines():
		l = l[:-1] # end line

		comp = l[0:2], l[2:4], l[4:6]
		rgb = tuple(
			int(c, 16)/255.0 for c in comp
		)
		t = "(R=%f, G=%f, B=%f, A=1.000000)"%rgb
		t2 = "Theme%iColor%i=%s"%(theme, count, t)
		t2 += "\nTheme%iLabel%i=\n" %(theme, count)
		o.write(t2)

		count += 1
		if count == m :
			count = 0
			theme += 1
			t3 = "Theme%i=Palette%i\n"%(theme,theme)
			o.write(t3)
		print(l, comp, rgb, t2)

if __name__=="__main__":
	main()
