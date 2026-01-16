import csv
import xml.etree.ElementTree as ET

xml_in  = "sumoTrace.xml"
csv_out = "sumoTrace.csv"

tree = ET.parse(xml_in)
root = tree.getroot()

rows = []
for ts in root.findall("timestep"):
    t = ts.get("time")
    for v in ts.findall("vehicle"):
        rows.append({
            "time": t,
            "id": v.get("id"),
            "x": v.get("x"),
            "y": v.get("y"),
            "speed": v.get("speed"),
            "angle": v.get("angle"),
            "lane": v.get("lane"),
            "pos": v.get("pos"),
        })

# choose columns you want (remove any you don't need)
fieldnames = ["time","id","x","y","speed","angle","lane","pos"]

with open(csv_out, "w", newline="", encoding="utf-8") as f:
    w = csv.DictWriter(f, fieldnames=fieldnames)
    w.writeheader()
    for r in rows:
        w.writerow(r)

print(f"Wrote {len(rows)} rows to {csv_out}")
