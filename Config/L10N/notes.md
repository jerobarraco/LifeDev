on development build files need to be on ./Linux/LifeDev/Config/L10N/Diags00.json (./Linux is where the build is)
on shipping builds i think it works too

just ignoring the nsloctext and leaving the raw text is fine
e.g.

"Text": "Una pagina:",

instead of
"Text": "NSLOCTEXT(\"[3F40862FE7193268CC653E3D0F0D983F]\", \"D64BD82CF8EC4D848EFFCAFAD469C6E7\", \"Una pagina:\")",

