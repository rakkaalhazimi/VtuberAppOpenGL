# VtuberAppCPP

## Run Program
```
sh build.sh <model_filename>
```

## Issues Along Developments
### Double library paths
Since I am using ucrt64, I need to exclude the /mingw64/bin from `PATH` or else my code can't compile due to library problems.  

### ImGUI Freeze when using single slider in CollapsingHeader
This will freeze
```cpp
if (ImGui::CollapsingHeader("Morphs"))
{
  ImGui::SliderFloat("slider float", &morphWeight, 0.0f, 1.0f, "ratio = %.3f");
}

```
This run fine
```cpp
if (ImGui::CollapsingHeader("Morphs"))
{
  ImGui::Text("Wink");
  ImGui::SliderFloat("slider float", &morphWeight, 0.0f, 1.0f, "ratio = %.3f");
}

```


