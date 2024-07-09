import { resolve } from 'path'
import { defineConfig } from 'vite'
import { nodeResolve } from '@rollup/plugin-node-resolve'

export default defineConfig({
  plugins: [nodeResolve()],
  server: {
    watch: {
      ignored: ['**/_opam']
    }
  },
  build: {
    rollupOptions: {
      input: {
        order_confirmation: resolve(__dirname, 'src/order-confirmation/index.html'),
      },
    },
  },
});