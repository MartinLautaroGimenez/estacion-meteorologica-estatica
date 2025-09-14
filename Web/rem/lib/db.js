import { readFile, writeFile } from "fs/promises";
import path from "path";

export async function readData(file) {
    const filePath = path.join(process.cwd(), "config", file);
    const data = await readFile(filePath, "utf8");
    return JSON.parse(data);
}

export async function saveData(file, data) {
    const filePath = path.join(process.cwd(), "config", file);
    await writeFile(filePath, JSON.stringify(data, null, 2));
}
